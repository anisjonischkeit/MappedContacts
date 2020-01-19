type emailAddress = {
  .
  "label": string,
  "email": string,
};
type phoneNumber = {
  .
  "label": string,
  "number": string,
};
type postalAddress = {
  .
  "label": string,
  "formattedAddress": string,
  "street": string,
  "pobox": string,
  "neighborhood": string,
  "city": string,
  "region": string,
  "state": string,
  "postCode": string,
  "country": string,
};
type birthday = {
  .
  "day": float,
  "month": float,
  "year": float,
};
type contact = {
  .
  "recordID": string,
  "backTitle": string,
  "company": string,
  "emailAddresses": array(emailAddress),
  "familyName": string,
  "givenName": string,
  "middleName": string,
  "jobTitle": string,
  "phoneNumbers": array(phoneNumber),
  "hasThumbnail": bool,
  "thumbnailPath": string,
  "postalAddresses": array(postalAddress),
  "prefix": string,
  "suffix": string,
  "department": string,
  "birthday": birthday,
};

[@bs.module "react-native-contacts"]
external getAll:
  (~callback: (Js.Nullable.t(string), array(contact)) => unit) => unit =
  "";
let getAll = (cb: Belt.Result.t(array(contact), string) => unit) => {
  Js.log("a");
  getAll(~callback=(e, r) => {
    Js.log2(e, r);
    cb(
      switch (Js.Nullable.toOption(e)) {
      | Some(e) =>
        Js.log("err");
        Belt.Result.Error(e);
      | None => Belt.Result.Ok(r)
      },
    );
  });
};

[@bs.module "index"]
external getAllWithoutPhotos: (~callback: 'any) => 'any = "";

[@bs.module "index"]
external getContactById: (~contactId: string, ~callback: 'any) => 'any = "";

[@bs.module "index"] external getCount: (~callback: 'any) => 'any = "";

[@bs.module "index"]
external getPhotoForId: (~contactId: string, ~callback: 'any) => 'any = "";

[@bs.module "index"]
external addContact: (~contact: contact, ~callback: 'any) => 'any = "";

[@bs.module "index"]
external openContactForm: (~contact: contact, ~callback: 'any) => 'any = "";

[@bs.module "index"]
external openExistingContact: (~contact: contact, ~callback: 'any) => 'any =
  "";

[@bs.module "index"]
external updateContact: (~contact: contact, ~callback: 'any) => 'any = "";

[@bs.module "index"]
external deleteContact: (~contact: contact, ~callback: 'any) => 'any = "";

[@bs.module "index"]
external getContactsMatchingString: (~str: string, ~callback: 'any) => 'any =
  "";

[@bs.module "index"]
external getContactsByPhoneNumber:
  (~phoneNumber: string, ~callback: 'any) => 'any =
  "";

[@bs.module "index"] external checkPermission: (~callback: 'any) => 'any = "";

[@bs.module "index"]
external requestPermission: (~callback: 'any) => 'any = "";

[@bs.module "index"]
external writePhotoToPath:
  (~contactId: string, ~file: string, ~callback: 'any) => 'any =
  "";