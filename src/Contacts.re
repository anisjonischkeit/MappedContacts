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

type contacts = array(contact);

let wrapCb = (cb, e, r) =>
  cb(
    switch (Js.Nullable.toOption(e)) {
    | Some(e) => Belt.Result.Error(e)
    | None => Belt.Result.Ok(r)
    },
  );

let wrapNullableResCb = (cb, e, r) =>
  cb(
    switch (Js.Nullable.toOption(e)) {
    | Some(e) => Belt.Result.Error(e)
    | None => Belt.Result.Ok(Js.Nullable.toOption(r))
    },
  );

[@bs.deriving jsConverter]
type permissionsStatus = [
  | [@bs.as "authorized"] `Authorized
  | [@bs.as "denied"] `Denied
  | [@bs.as "undefined"] `Undefined
];

type jsCallback('a) = (Js.Nullable.t(string), 'a) => unit;
type callback('a) = Belt.Result.t('a, string) => unit;

[@bs.module "react-native-contacts"]
external getAll: (~callback: jsCallback(contacts)) => unit = "";
let getAll = cb => getAll(~callback=cb->wrapCb);

[@bs.module "react-native-contacts"]
external getAllWithoutPhotos: (~callback: jsCallback(contacts)) => unit = "";
let getAllWithoutPhotos = cb => getAllWithoutPhotos(cb->wrapCb);

[@bs.module "react-native-contacts"]
external getContactById:
  (~contactId: string, ~callback: jsCallback(contact)) => unit =
  "";
let getContactById = (contactId, cb) =>
  getContactById(contactId, cb->wrapCb);

[@bs.module "react-native-contacts"]
external getCount: (~callback: jsCallback(int)) => unit = "";
let getCount = cb => getCount(cb->wrapCb);

[@bs.module "react-native-contacts"]
external getPhotoForId:
  (~contactId: string, ~callback: jsCallback(string)) => unit =
  "";
let getPhotoForId = (contactId, cb) => getPhotoForId(contactId, cb->wrapCb);

[@bs.module "react-native-contacts"]
external addContact:
  (~contact: contact, ~callback: jsCallback(contact)) => unit =
  "";
let addContact = (contact, cb) => addContact(contact, cb->wrapCb);

[@bs.module "react-native-contacts"]
external openContactForm:
  (~contact: createContact, ~callback: jsCallback(Js.Nullable.t(contact))) =>
  unit =
  "";
let openContactForm = (contact, cb) =>
  openContactForm(contact, cb->wrapNullableResCb);

[@bs.module "react-native-contacts"]
external openExistingContact:
  (~contact: contact, ~callback: jsCallback(option(contact))) => unit =
  "";
let openExistingContact = (contact, cb) =>
  openExistingContact(contact, cb->wrapCb);

[@bs.module "react-native-contacts"]
external viewExistingContact:
  (~contact: contact, ~callback: jsCallback(option(contact))) => unit =
  "";
let viewExistingContact = (contact, cb) =>
  viewExistingContact(contact, cb->wrapCb);

[@bs.module "react-native-contacts"]
external openContactPicker:
  (~callback: jsCallback(Js.Nullable.t(contact))) => unit =
  "";
let openContactPicker = cb => openContactPicker(cb->wrapNullableResCb);

[@bs.module "react-native-contacts"]
external updateContact:
  (~contact: contact, ~callback: jsCallback(option(contact))) => unit =
  "";
let updateContact = (contact, cb) => updateContact(contact, cb->wrapCb);

[@bs.module "react-native-contacts"]
external deleteContact:
  (~contact: contact, ~callback: jsCallback(unit)) => unit =
  "";
let deleteContact = (contact, cb) => deleteContact(contact, cb->wrapCb);

[@bs.module "react-native-contacts"]
external getContactsMatchingString:
  (~str: string, ~callback: jsCallback(contacts)) => unit =
  "";
let getContactsMatchingString = (str, cb) =>
  getContactsMatchingString(str, cb->wrapCb);

[@bs.module "react-native-contacts"]
external getContactsByPhoneNumber:
  (~phoneNumber: string, ~callback: jsCallback(contacts)) => unit =
  "";
let getContactsByPhoneNumber = (phoneNumber, cb) =>
  getContactsByPhoneNumber(phoneNumber, cb->wrapCb);

[@bs.module "react-native-contacts"]
external checkPermission: (~callback: jsCallback(permissionsStatus)) => unit =
  "";
let checkPermission = cb => checkPermission(cb->wrapCb);

[@bs.module "react-native-contacts"]
external requestPermission: (~callback: jsCallback(permissionsStatus)) => unit =
  "";
let requestPermission = cb => requestPermission(cb->wrapCb);

[@bs.module "react-native-contacts"]
external writePhotoToPath:
  (~contactId: string, ~file: string, ~callback: jsCallback(bool)) => unit =
  "";
let writePhotoToPath = (contactId, file, cb) =>
  writePhotoToPath(contactId, file, cb->wrapCb);