type emailAddress = {
  label: string,
  email: string,
};
type phoneNumber = {
  label: string,
  number: string,
};

type postalAddress = {
  label: string,
  formattedAddress: string,
  street: string,
  pobox: string,
  neighborhood: string,
  city: string,
  region: string,
  state: string,
  postCode: string,
  country: string,
};
type birthday = {
  day: float,
  month: float,
  year: float,
};

[@bs.deriving abstract]
type createContact = {
  [@bs.optional]
  recordID: string,
  [@bs.optional]
  backTitle: string,
  [@bs.optional]
  company: string,
  [@bs.optional]
  emailAddresses: array(emailAddress),
  [@bs.optional]
  familyName: string,
  [@bs.optional]
  givenName: string,
  [@bs.optional]
  middleName: string,
  [@bs.optional]
  jobTitle: string,
  [@bs.optional]
  phoneNumbers: array(phoneNumber),
  [@bs.optional]
  hasThumbnail: bool,
  [@bs.optional]
  thumbnailPath: string,
  [@bs.optional]
  postalAddresses: array(postalAddress),
  [@bs.optional]
  prefix: Js.Nullable.t(string),
  [@bs.optional]
  suffix: Js.Nullable.t(string),
  [@bs.optional]
  department: string,
  [@bs.optional]
  birthday,
};

type contact = {
  recordID: string,
  rawContactId: string,
  backTitle: string,
  company: string,
  emailAddresses: array(emailAddress),
  familyName: string,
  givenName: string,
  middleName: string,
  jobTitle: string,
  phoneNumbers: array(phoneNumber),
  hasThumbnail: bool,
  thumbnailPath: string,
  postalAddresses: array(postalAddress),
  prefix: Js.Nullable.t(string),
  suffix: Js.Nullable.t(string),
  department: string,
  birthday,
};
type permissionsStatus = [ | `Authorized | `Denied | `Undefined];
type callback('a) = Belt.Result.t('a, string) => unit;

let getAll: callback(array(contact)) => unit;
let getAllWithoutPhotos: callback(array(contact)) => unit;
let getContactById: (string, callback(contact)) => unit;
let getCount: callback(int) => unit;
let getPhotoForId: (string, callback(string)) => unit;
let addContact: (contact, callback(contact)) => unit;
// should be Js.Nullbale,t
let openContactForm: (createContact, callback(option(contact))) => unit;
let openExistingContact: (contact, callback(option(contact))) => unit;
let viewExistingContact: (contact, callback(option(contact))) => unit;
let openContactPicker: callback(option(contact)) => unit;
let updateContact: (contact, callback(option(contact))) => unit;
let deleteContact: (contact, callback(unit)) => unit;
let getContactsMatchingString: (string, callback(array(contact))) => unit;
let getContactsByPhoneNumber: (string, callback(array(contact))) => unit;
let checkPermission: callback(permissionsStatus) => unit;
let requestPermission: callback(permissionsStatus) => unit;
let writePhotoToPath: (string, string, callback(bool)) => unit;