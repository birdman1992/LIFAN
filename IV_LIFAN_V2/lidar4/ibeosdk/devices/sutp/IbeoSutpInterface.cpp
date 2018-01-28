//======================================================================
/** \file IbeoSutpInterface.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Aug 31, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/devices/sutp/IbeoSutpInterface.hpp>

#include <ibeosdk/datablocks/snippets/SutpHeader.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/Log.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/streambuf.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint64_t IbeoSutpInterface::sutpDefaultTimeout = 80; // [ms]
const uint64_t IbeoSutpInterface::sutpFragmentSize = 8024; // incl Hdr
const uint32_t IbeoSutpInterface::defaultErrorResetLimit = 10;

//======================================================================

IbeoSutpInterface::IbeoSutpInterface (MessageHandler msgHandler,
                                      uint64_t timeout,
                                      uint32_t errResetLimit)
  : m_sutpTimeout(timeout),
    m_errorResetLimit(errResetLimit),
    m_fragmentCache(),
    m_nextSeqNbRx(0),
    m_nextSeqNbRxInvalid(true),
    m_sutpTime(0),
    m_paketCacheMutex(),
    m_paketComplete(),
    m_numConsecutiveErrors(0),
    m_onReceive(msgHandler)
{
	m_fragmentCache.rehash(size_t(std::numeric_limits<uint16_t>::max()>>1));
}

//======================================================================

void IbeoSutpInterface::reset()
{
	std::cout << "Resetting SUTP Protocol" << std::endl;
	m_fragmentCache.clear();
	m_nextSeqNbRx = 0;
	m_nextSeqNbRxInvalid = true;
	m_sutpTime = 0;
	m_numConsecutiveErrors = 0;
}

//==============================================================================

/**
 * Closes the connection if necessary
 */
IbeoSutpInterface::~IbeoSutpInterface()
{
}

//======================================================================

void IbeoSutpInterface::setMessageHandler(IbeoSutpInterface::MessageHandler msgHandler)
{
	m_onReceive = msgHandler;
}

//======================================================================

void IbeoSutpInterface::deregisterMessageHandler()
{
	m_onReceive = NULL;
}

//======================================================================

bool IbeoSutpInterface::onReceiveRaw(std::istream& is, const uint32_t messageSize)
{

	if (!is || (!is.good())) {
		return false;
	}

	if (messageSize < uint32_t(SutpHeader::getSerializedSize_Static())) {
		return false;
	}


	// read header
	boost::shared_ptr<SutpHeader> sh(new SutpHeader());
	sh->read(is);
	if (!is || (!is.good())) {
		logWarning << "Error reading SUTP header. Dropping paket." << std::endl;
		++m_numConsecutiveErrors;
		if (m_numConsecutiveErrors > m_errorResetLimit) {
			reset();
		}
		return false;
	}

	boost::posix_time::ptime tmpTs
	        = boost::posix_time::microsec_clock::local_time();
	sh->setTimestamp(uint64_t((tmpTs-
			boost::posix_time::ptime(boost::gregorian::date(1950,boost::gregorian::Dec,1)))
			.total_milliseconds()));
	m_sutpTime = std::max(m_sutpTime, sh->getTimestamp());
	if (   m_fragmentCache.find(sh->getSeqNo()) != m_fragmentCache.end()
	    || (!m_nextSeqNbRxInvalid && (INT16(sh->getSeqNo() - m_nextSeqNbRx) < 0)))
	{
		logWarning << "Received sequence number twice or out of order: "
		           << sh->getSeqNo() <<". Dropping paket." << std::endl;
		++m_numConsecutiveErrors;
		if (m_numConsecutiveErrors > m_errorResetLimit) {
			reset();
		}
		return false;
	}

	// read payload
	uint32_t payloadSize = messageSize - uint32_t(sh->getSerializedSize());
	boost::shared_ptr<std::vector<uint8_t> > payload(new std::vector<uint8_t>(payloadSize));
	payload->resize(payloadSize);
	if(payloadSize > 0)
		is.read((char*)(&((*payload)[0])), payloadSize);

	if (!is || (!is.good())) {
		logWarning << "Failed to get SUTP payload. dropping paket."
		           << std::endl;
		++m_numConsecutiveErrors;
		if (m_numConsecutiveErrors > m_errorResetLimit) {
			reset();
		}
		return false;
	}

	// first fragment ever? --> set last used seqNo
	if (m_nextSeqNbRxInvalid)
	{
		m_nextSeqNbRx = uint16_t(sh->getSeqNo() - (sh->getFragNo() - 1));
		m_nextSeqNbRxInvalid = false;
	}

	// cache fragment
	SudpFragment frag(sh, payload);
	{ // mutexed
		boost::mutex::scoped_lock lock(m_paketCacheMutex);
		m_fragmentCache[sh->getSeqNo()] = frag;
		m_paketComplete.notify_all();
	} // mutexed

	return true;
}

//======================================================================

void IbeoSutpInterface::processCache()
{
	while (isPacketComplete()) {
		processPaket();
	}

	clearOutdatedFragments();

	while (isPacketComplete()) {
		processPaket();
	}
}

//======================================================================

void IbeoSutpInterface::processPaket()
{
	if (m_nextSeqNbRxInvalid) {
		return;
	}

	boost::asio::streambuf buf;
	size_t size = 0;
	bool complete = false;
	uint16_t i=0;

	{ // mutexed
		boost::mutex::scoped_lock lock(m_paketCacheMutex);
		for (i=m_nextSeqNbRx; !complete; i = uint16_t(i+1)) {
			buf.sputn((const char*)(&((*(m_fragmentCache[i].second))[0])),
			          std::streamsize(m_fragmentCache[i].second->size()));
			size += m_fragmentCache[i].second->size();
			if (   m_fragmentCache[i].first->getFragNo()
			    == m_fragmentCache[i].first->getFragsTotal())
			{
				complete = true;
			}
		}
		// Drop used fragments
		for(; m_nextSeqNbRx != i; m_nextSeqNbRx = uint16_t(m_nextSeqNbRx+1)) {
			m_fragmentCache.erase(m_nextSeqNbRx);
		}
	} // mutexed

	buf.commit(size);
	std::istream is(&buf);

	try {
		if (m_onReceive) {
			m_onReceive(is, size);
		}
	}
	catch (const std::exception& e) {
		logWarning << "exception during onReceive Callback: " << e.what()
		           << std::endl;
	}

	m_numConsecutiveErrors = 0;
}

//======================================================================

bool IbeoSutpInterface::isPacketComplete()
{

	bool complete = false;
	if (!m_nextSeqNbRxInvalid)
	{
		uint16_t i = m_nextSeqNbRx;
		while (    (!complete)
				&& (m_fragmentCache.find(i) != m_fragmentCache.end()))
		{
			if (   m_fragmentCache[i].first->getFragNo()
			    == m_fragmentCache[i].first->getFragsTotal())
			{
				complete = true;
			}
			i = uint16_t(i+1);
		}
	}

	return complete;
}

//======================================================================

void IbeoSutpInterface::clearOutdatedFragments()
{
	if (m_nextSeqNbRxInvalid)
	{
		return;
	}

	boost::unordered_map<uint16_t, SudpFragment>::iterator it
	        = m_fragmentCache.begin();
	for (; it != m_fragmentCache.end();)
	{
		if ((m_sutpTime - it->second.first->getTimestamp()) > m_sutpTimeout)
		{

			bool ibeoDataPacketLost = false;
			//wrap around handling
			if ( INT16(it->first - m_nextSeqNbRx) >= 0 )
			{
				uint16_t paketStartSeqNo
				        = uint16_t((it->first
				                    - (it->second.first->getFragNo()-1U))
				                   & 0xFFFFU);
				m_nextSeqNbRx = uint16_t(
				            uint16_t(paketStartSeqNo
				                     + it->second.first->getFragsTotal())
				            & 0xFFFFU);
				ibeoDataPacketLost = true;
			}
			logWarning << "Paket timed out (seqNo=" << it->first
			           << "). Dropping paket." << std::endl;
			boost::unordered_map<uint16_t, SudpFragment>::iterator delIt = it;
			++it;
			m_fragmentCache.erase(delIt);
			if (ibeoDataPacketLost)
			{
				++m_numConsecutiveErrors;
				if (m_numConsecutiveErrors > m_errorResetLimit)
				{
					reset();
				}
				break;
			}
		} else {
			++it;
		}
	}
}

//======================================================================

} // namespace ibeo

//======================================================================
