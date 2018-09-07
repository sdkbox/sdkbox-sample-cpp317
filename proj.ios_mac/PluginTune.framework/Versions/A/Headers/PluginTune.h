/****************************************************************************

 Copyright (c) 2014-2016 SDKBOX Inc

 ****************************************************************************/

#ifndef _PLUGIN_TUNE_H_
#define _PLUGIN_TUNE_H_

#include <map>
#include <vector>
#include <string>

#include "sdkbox/Sdkbox.h"

namespace sdkbox
{
    struct TunePreloadData
    {

        /*!
         * Name or ID of the publisher on MAT
         */
        std::string publisherId;

        /*!
         * ID of the offer in the MAT platform
         */
        std::string offerId;

        /*!
         * Name of agency connected to the advertiser account in the MAT platform
         */
        std::string agencyId;

        /*!
         * Reference ID of the publisher
         */
        std::string publisherReferenceId;

        /*!
         * First optional additional info string param related to the publisher
         */
        std::string publisherSub1;

        /*!
         * Second optional additional info string param related to the publisher
         */
        std::string publisherSub2;

        /*!
         * Third optional additional info string param related to the publisher
         */
        std::string publisherSub3;

        /*!
         * Fourth optional additional info string param related to the publisher
         */
        std::string publisherSub4;

        /*!
         * Fifth optional additional info string param related to the publisher
         */
        std::string publisherSub5;

        /*!
         * Name or ID of an ad in campaign on partner’s platform
         */
        std::string publisherSubAd;

        /*!
         * ID of the ad group in campaign on partner’s platform
         */
        std::string publisherSubAdgroup;

        /*!
         * Name or ID of the campaign on in partner’s platform
         */
        std::string publisherSubCampaign;

        /*!
         * Name or ID of the keyword specific to Google AdWords integration and other search campaigns
         */
        std::string publisherSubKeyword;

        /*!
         * Name or ID of the down-stream publisher
         */
        std::string publisherSubPublisher;

        /*!
         * Name or ID of the site or mobile app the campaign is in/on
         */
        std::string publisherSubSite;

        /*!
         * Value of "advertiser_sub_ad" passed into measurement URL on click.
         */
        std::string advertiserSubAd;

        /*!
         * Value of "sub_adgroup" passed into measurement URL on click.
         */
        std::string advertiserSubAdgroup;

        /*!
         * Value of "advertiser_sub_campaign" passed into measurement URL on click.
         */
        std::string advertiserSubCampaign;

        /*!
         * Value of "advertiser_sub_keyword" passed into measurement URL on click.
         */
        std::string advertiserSubKeyword;

        /*!
         * Value of "advertiser_sub_publisher" passed into measurement URL on click.
         */
        std::string advertiserSubPublisher;

        /*!
         Value of "advertiser_sub_site" passed into measurement URL on click.
         */
        std::string advertiserSubSite;
    }; // TunePreloadData

    struct TuneEventItem
    {
        TuneEventItem() : unitPrice(.0f), quantity(0), revenue(.0f) {}

        /*!
         * name of the event item
         */
        std::string item;
        /*!
         * unit price of the event item
         */
        float unitPrice;
        /*!
         * quantity of the event item
         */
        int quantity;
        /*!
         * revenue of the event item
         */
        float revenue;
        /*!
         * an extra parameter that corresponds to attribute_sub1 property of the event item
         */
        std::string attribute1;
        /*!
         * an extra parameter that corresponds to attribute_sub2 property of the event item
         */
        std::string attribute2;
        /*!
         * an extra parameter that corresponds to attribute_sub3 property of the event item
         */
        std::string attribute3;
        /*!
         * an extra parameter that corresponds to attribute_sub4 property of the event item
         */
        std::string attribute4;
        /*!
         * an extra parameter that corresponds to attribute_sub5 property of the event item
         */
        std::string attribute5;
    };

    struct TuneEvent
    {
        TuneEvent() : eventId(0), revenue(.0f), transactionState(0), rating(.0f), level(0)
                    , quantity(0), timeIntervalSince1970Date1(0), timeIntervalSince1970Date2(0)
        {}

        /*!
         * Name of the event
         */
        std::string eventName;

        /*!
         * Event ID of the event as defined on the MobileAppTracking dashboard
         */
        long eventId;

        /*!
         * An array of MATEventItem items
         */
        std::vector<TuneEventItem> eventItems;

        /*!
         * Revenue associated with the event
         */
        float revenue;

        /*!
         * Currency code associated with the event
         */
        std::string currencyCode;

        /*!
         * Reference ID associated with the event
         */
        std::string refId;

        /*!
         * App Store in-app-purchase transaction receipt data or Google Play receipt data
         */
        std::string receipt;

        /*!
         * Google Play receipt signature (Only)
         */
         std::string receiptSignature;

        /*!
         * Content type associated with the event (e.g., @"shoes")
         */
        std::string contentType;

        /*!
         * Content ID associated with the event (International Article Number
         * (EAN) when applicable, or other product or content identifier)
         */
        std::string contentId;

        /*!
         * Search string associated with the event
         */
        std::string searchString;

        /*!
         * Transaction state of App Store in-app-purchase
         */
        long transactionState;

        /*!
         * Rating associated with the event (e.g., a user rating an item)
         */
        float rating;

        /*!
         * Level associated with the event (e.g., for a game)
         */
        long level;

        /*!
         * Quantity associated with the event (e.g., number of items)
         */
        unsigned long quantity;

        /*!
         * First date associated with the event (e.g., user's check-in time)
         */
        unsigned long timeIntervalSince1970Date1;

        /*!
         * Second date associated with the next action (e.g., user's check-out time)
         */
        unsigned long timeIntervalSince1970Date2;

        /*!
         * First custom string attribute for the event
         */
        std::string  attribute1;

        /*!
         * Second custom string attribute for the event
         */
        std::string  attribute2;

        /*!
         * Third custom string attribute for the event
         */
        std::string attribute3;

        /*!
         * Fourth custom string attribute for the event
         */
        std::string attribute4;

        /*!
         * Fifth custom string attribute for the event
         */
        std::string attribute5;
    }; // TuneEvent


    class TuneListener
    {
    public:
        virtual void onMobileAppTrackerEnqueuedActionWithReferenceId(const std::string &referenceId) = 0;
        virtual void onMobileAppTrackerDidSucceedWithData(const std::string &data) = 0;
        virtual void onMobileAppTrackerDidFailWithError(const std::string &errorString) = 0;
        virtual void onMobileAppTrackerDidReceiveDeeplink(const std::string &deeplink, bool timeout) = 0;
        virtual void onMobileAppTrackerDidFailDeeplinkWithError(const std::string &errorString) = 0;
    }; // TuneListener

    class PluginTune
    {
    public:

        enum Gender
        {
            GenderMale       = 0,                // Gender type MALE. Equals 0.
            GenderFemale     = 1                 // Gender type FEMALE. Equals 1.
        };

        /*!
         * set a listener to listen for event changes.
         */
        static void setListener(TuneListener* listener);

        /*!
         * Get the listener
         */
        static TuneListener* getListener();

        /*!
         * remove the listener, and can't listen to events anymore
         */
        static void removeListener();

        ~PluginTune();

        /*!
         * init the instance.
         */
        static void init();

        /*!
         * Specifies that the server responses should include debug information.
         *
         * @warning This is only for testing. You must turn this off for release builds.
         * @param enable defaults to false.
         */
        static void setDebugMode(bool enable);

        /*!
         * Set to YES to allow duplicate requests to be registered with the MAT server.
         *
         * @warning This is only for testing. You must turn this off for release builds.
         * @param allow defaults to false.
         */
        static void setAllowDuplicateRequests(bool allow);

        /*!
         * Check for a deferred deeplink entry point upon app installation.
         * This is safe to call at every app launch, since the function does nothing
         * unless this is the first launch.
         *
         * The timeout parameter should be set in keeping with the normal first-launch
         * time and user experience of your app.
         *
         * @param timeout If the deeplink value is not received within this timeout duration, then the deeplink will not be opened.
         */
        SDKBOX_DEPRECATED("checkForDeferredDeepLink") static void checkForDeferredDeeplinkWithTimeout(double timeout);

        /*!
         * Check for a deferred deeplink entry point upon app installation.
         * This is safe to call at every app launch, since the function does nothing
         * unless this is the first launch.
         */
        static void checkForDeferredDeepLink();

        /*!
         * Enable automatic measurement of app store in-app-purchase events. When enabled, your code
         * should not explicitly measure events for successful purchases related to StoreKit to avoid event duplication.
         *
         * @param automate Automate IAP purchase event measurement. Defaults to false.
         * @platform iOS
         */
        static void automateIapEventMeasurement(bool automate);

        /*!
         * Set whether the MAT events should also be logged to the Facebook SDK. This flag is ignored
         * if the Facebook SDK is not present.
         *
         * @param logging Whether to send MAT events to FB as well
         * @param limit Whether data such as that generated through FBAppEvents and sent to Facebook
         * should be restricted from being used for other than analytics and conversions.  Defaults to false.
         * This value is stored on the device and persists across app launches.
         */
        static void setFacebookEventLogging(bool logging, bool limit);

        /*!
         * Set whether this is an existing user or a new one. This is generally used to
         * distinguish users who were using previous versions of the app, prior to
         * integration of the MAT SDK. The default is to assume a new user.
         *
         * See http://support.mobileapptracking.com/entries/22621001-Handling-Installs-prior-to-SDK-implementation
         *
         * @param existingUser - Is this a pre-existing user of the app? Default: false
         */
        static void setExistingUser(bool existingUser);

        /*!
         * Set the Apple Advertising Identifier available in iOS 6.
         *
         * @param appleAdvertisingIdentifier - Apple Advertising Identifier
         * @platform iOS
         */
        static void setAppleAdvertisingIdentifier(const std::string &appleAdvertisingIdentifier,
                                                  bool adTrackingEnabled);

        /*!
         * Set the Apple Vendor Identifier available in iOS 6.
         *
         * @param appleVendorIdentifier - Apple Vendor Identifier
         * @platform iOS
         */
        static void setAppleVendorIdentifier(const std::string &appleVendorIdentifier);

        /*!
         * Sets the currency code.
         *
         * Default: USD
         *
         * @param currencyCode The string name for the currency code.
         */
        static void setCurrencyCode(const std::string &currencyCode);

        /*!
         * Sets the jailbroken device flag.
         *
         * @param jailbroken The jailbroken device flag.
         * @platform iOS
         */
        static void setJailbroken(bool jailbroken);

        /*!
         * Sets the package name (bundle identifier).
         * Defaults to the Bundle Identifier of the app that is running the sdk.
         *
         * @param packageName The string name for the package.
         */
        static void setPackageName(const std::string &packageName);

        /*!
         * Specifies if the sdk should auto detect if the iOS device is jailbroken.
         *
         * @param autoDetect true will detect if the device is jailbroken, defaults to true.
         */
        static void setShouldAutoDetectJailbroken(bool autoDetect);

        /*!
         * Specifies if the sdk should pull the Apple Vendor Identifier from the device.
         * Note that setting to false will clear any previously set value for the property.
         *
         * @param autoGenerate true will set the Apple Vendor Identifier, defaults to true.
         * @platform iOS
         */
        static void setShouldAutoGenerateAppleVendorIdentifier(bool autoGenerate);

        /*!
         * Sets the site ID.
         *
         * @param siteId The MAT app/site ID of this mobile app.
         */
        SDKBOX_DEPRECATED("setPackageName") static void setSiteId(const std::string &siteId);

        /*!
         * Set the TRUSTe Trusted Preference Identifier (TPID).
         *
         * @param tpid - Trusted Preference Identifier
         */
        static void setTRUSTeId(const std::string &tpid);

        /*!
         * Sets the user's email address.
         *
         * @param userEmail The user's email address.
         */
        static void setUserEmail(const std::string &userEmail);

        /*!
         * Sets the user ID.
         *
         * @param userId The string name for the user ID.
         */
        static void setUserId(const std::string &userId);

        /*!
         * Sets the user's name.
         *
         * @param userName The user's name.
         */
        static void setUserName(const std::string &userName);

        /*!
         * Sets the user's phone number.
         *
         * @param phoneNumber The user's phone number.
         */
        static void setPhoneNumber(const std::string &phoneNumber);

        /*!
         * Set user's Facebook ID.
         *
         * @param facebookUserId string containing the user's Facebook user ID.
         */
        static void setFacebookUserId(const std::string &facebookUserId);

        /*!
         * Set user's Twitter ID.
         *
         * @param twitterUserId string containing the user's Twitter user ID.
         */
        static void setTwitterUserId(const std::string &twitterUserId);

        /*!
         * Set user's Google ID.
         *
         * @param googleUserId string containing the user's Google user ID.
         */
        static void setGoogleUserId(const std::string &googleUserId);

        /*!
         * Sets the user's age.
         *
         * @param userAge user's age
         */
        static void setAge(int userAge);

        /*!
         * Sets the user's gender.
         *
         * @param userGender user's gender, possible values GenderMale (0), GenderFemale (1)
         */
        static void setGender(Gender userGender);

        /*!
         * Sets the user's location.
         *
         * @param latitude user's latitude
         * @param longitude user's longitude
         */
        static void setLatitude(double latitude, double longitude);

        /*!
         * Sets the user's location including altitude.
         *
         * @param latitude user's latitude
         * @param longitude user's longitude
         * @param altitude user's altitude
         */
        static void setLatitude(double latitude, double longitude, double altitude);

        /*!
         * Set app-level ad-tracking.
         *
         * @param enable true means opt-in, false means opt-out.
         */
        static void setAppAdTracking(bool enable);

        /*!
         * Set whether the user is generating revenue for the app or not.
         * If measureEvent is called with a non-zero revenue, this is automatically set to YES.
         *
         * @param isPayingUser true if the user is revenue-generating, false if not
         */
        static void setPayingUser(bool isPayingUser);

        /*!
         * Sets publisher information for attribution.
         *
         * @function setPreloadDataForScript for Js / Lua
         * @function setPreloadData for cpp
         *
         * @param preloadData Preload app attribution data
         */
        static void setPreloadData(const TunePreloadData &preloadData);
        static void setPreloadDataForScript(const std::string &jsonString);

        /*!
         * Get the MAT ID for this installation (mat_id).
         *
         * @return MAT ID
         */
        SDKBOX_DEPRECATED("tuneId") static std::string matId();

        /*!
         * Get the Tune ID for this installation.
         *
         * @return Tune ID
         */
        static std::string tuneId();

        /*!
         * Get the MAT log ID for the first app open (open_log_id).
         *
         * @return open log ID
         */
        static std::string openLogId();

        /*!
         * Get whether the user is revenue-generating.
         *
         * @return YES if the user has produced revenue, NO if not
         */
        static bool isPayingUser();

        /*!
         * To be called when an app opens; typically in the AppDelegate::applicationWillEnterForeground() event.
         */
        static void measureSession();

        /*!
         * Record an event for an Event Name.
         *
         * @param eventName The event name.
         */
        static void measureEventName(const std::string &eventName);

        /*!
         * Record an event by providing the equivalent Event ID defined on the MobileAppTracking dashboard.
         *
         * @param eventId The event ID.
         */
        static void measureEventId(int eventId);

        /*!
         * Record an event with a MATEvent.
         *
         * @function measureEventForScript for Js / Lua
         * @function measureEvent for cpp
         *
         * @param event The MATEvent.
         */
        static void measureEvent(const TuneEvent &event);
        static void measureEventForScript(const std::string &jsonString);

        /*!
         * Sets whether or not to use cookie based tracking.
         *
         * @param enable true/false for cookie based tracking.
         */
        static void setUseCookieTracking(bool enable);

        /*!
         * Sets a url to be used with app-to-app tracking so that
         * the sdk can open the download (redirect) url. This is
         * used in conjunction with the setTracking:advertiserId:offerId:publisherId:redirect: method.
         *
         * @param redirectUrl The string name for the url.
         */
        static void setRedirectUrl(const std::string &redirectUrl);

        /*!
         * Start an app-to-app tracking session on the MAT server.
         *
         * @param targetAppPackageName The bundle identifier of the target app.
         * @param targetAppAdvertiserId The MAT advertiser ID of the target app.
         * @param targetAdvertiserOfferId The MAT offer ID of the target app.
         * @param targetAdvertiserPublisherId The MAT publisher ID of the target app.
         * @param shouldRedirect Should redirect to the download url if the tracking session was
         * successfully created. See setRedirectUrl:.
         */
        static void startAppToAppTracking(const std::string &targetAppPackageName,
                                          const std::string &targetAppAdvertiserId,
                                          const std::string &targetAdvertiserOfferId,
                                          const std::string &targetAdvertiserPublisherId,
                                          bool shouldRedirect);


        /*!
         * Record the URL and Source when an application is opened via a URL scheme.
         * This typically occurs during OAUTH or when an app exits and is returned
         * to via a URL. The data will be sent to the HasOffers server when the next
         * measureXXX method is called so that a Re-Engagement can be recorded.
         *
         * @param urlString the url string used to open your app.
         * @param sourceApplication the source used to open your app. For example, mobile safari.
         */
        static void applicationDidOpenURL(const std::string &urlString, const std::string &sourceApplication);

        /*!
         * Record the URL and Source when an application is opened via a URL scheme.
         *
         * @param deepLinkUrl the url string used to open your app.
         */
        static void setDeepLink(const std::string &deepLinkUrl);

        #ifdef OBFUSCATED
        _fde6d02d541f4f1ab57ec133f7e74435
        #endif
    };
}

#endif // _PLUGIN_TUNE_H_
