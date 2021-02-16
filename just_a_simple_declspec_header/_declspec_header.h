#ifndef TEST_H
#define TEST_H

#ifdef TEST_API
#define TEST(type) __declspec(dllexport) type WINAPI
#else
#define TEST(type) extern __declspec(dllimport) type WINAPI
#endif // SECUREEMAIL_API

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace MainNameSpace
{
   /// Namespace that holds the enumeration of email methods and its string correspondent
   namespace Methods
   {
      enum Method
      {
         TO,
         CC,
         END
      };
      static const char *strMethod[2] = { "TO:", "CC:" };
   }

   namespace AddressBook
   {
      /// Load the address book dialog
      /// @return TRUE if the dialog is loaded and in either case if we have selected anything or not
      /// @param[in] emailList - this is a reference value to a std::map that will hold the selected recipients address
      ///                      - the string in the map is the key - the email destination (this will be unique per map)
      ///                      - the enum Method defines the method types (ex: TO = 0, CC = 1)
      TEST(bool)
         LoadAddressBook(std::map<std::string, MainNameSpace::Methods::Method> &emailList);

      /// Get already saved favorties.
      /// @return TRUE/FALSE
      TEST(bool)
         GetFavoriteEmails(std::map<std::string, MainNameSpace::Methods::Method> &emailList);

      /// Find out if the user has access to Direct Address Book based on Hnauser setup
      /// @return TRUE if the user has Secure Email
      /// @param[in] userID - if we send in the user if then we check that particular user
      ///         - if there is no user ID passed in we just check current user
      TEST(bool)
         CanSecureMessage();
   } // namespace AddressBook

   namespace SecureMessaging
   {
      /// Send an email via FSI (attachement saved under CAMM)
      /// @return TRUE if the FSI object was loaded so that the request can be made
      /// @param[in] emailList - the recipients address that can be gathered via LoadAddressBook function
      /// @param[in] subject - a string that will be set as the subject of the message
      /// @param[in] messageRTF - a RTF encoded string that will be set as the email message
      /// @param[out] result - message back the called with the request status
      /// @param[in] attachementList - a vector of string that will contain the attachment physical location
      TEST(bool)
         SendEmail(std::map<std::string, MainNameSpace::Methods::Method> emailList,
            std::string subject,
            std::string messageRTF,
            std::string &result,
            std::vector<std::string> attachementList);

      /// Load SendEmail Dialog
      /// @return TRUE if the FSI object was loaded so that the request can be made
      /// @param[in] emailList - the recipients address that can be gathered via LoadAddressBook function
      /// @param[in] subject - a string that will be set as the subject of the message
      /// @param[in] messageRTF - a RTF encoded string that will be set as the email message
      /// @param[out] result - message back the called with the request status
      /// @param[in] attachementList - a vector of string that will contain the attachment physical location
      TEST(bool)
         LoadSendEmailDialog(std::string &result,
            std::map<std::string, MainNameSpace::Methods::Method> emailList,
            std::string subject,
            std::string messageRTF,
            std::vector<std::string> attachementList);

   } // namespace SecureMessaging
} // namespace MainNameSpace

#endif