//======================================================================
/*! \file IbeoSDK.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date April 4, 2012
 *
 * Ibeo SDK main header file
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IBEOSDK_HPP_SEEN
#define IBEOSDK_IBEOSDK_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/misc/Version.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class IbeoSDK {
public:
	IbeoSDK() {}
	virtual ~IbeoSDK() {}

	static ibeosdk::Version getVersion()
	{
		return ibeosdk::Version(ibeosdk::Version::MajorVersion(5),
		                        ibeosdk::Version::MinorVersion(0),
		                        ibeosdk::Version::Revision(4));
	}
}; // IbeoSDK

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOSDK_HPP_SEEN

//======================================================================

