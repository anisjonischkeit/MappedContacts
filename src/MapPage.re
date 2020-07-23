open ReactNative;
open Contacts;
open Belt.Array;

[@react.component]
let make = (~navigation, ~route, ~location) => {
  let (contactsData, setContacts) = React.useState(() => None);
  React.useEffect0(() => {
    Contacts.getAllWithoutPhotos(contacts => {
      setContacts(_ => {Some(contacts)})
    });

    None;
  });

  <Map
    markers={
      switch (contactsData) {
      | Some(Ok(contacts)) =>
        contacts
        ->Belt.Array.map((contact: contact) => {
            contact.postalAddresses
            |> Array.map(address =>
                 switch (
                   address.label,
                   String.split_on_char(',', address.street),
                 ) {
                 | ("added at", [lat, lon, ..._]) =>
                   switch (
                     Belt.Float.fromString(lat),
                     Belt.Float.fromString(lon),
                   ) {
                   | (Some(lat), Some(lon)) =>
                     Some(
                       Map.{
                         latitude: lat,
                         longitude: lon,
                         title: contact.givenName,
                         id: contact.recordID,
                       },
                     )
                   | _ => None
                   }
                 | _ => None
                 }
               )
          })
        ->Belt.Array.concatMany
        ->Belt.Array.reduce([||], (acc, optMarker) =>
            switch (optMarker) {
            | Some(marker) => acc->Belt.Array.concat([|marker|])
            | None => acc
            }
          )
      | None => [||]
      }
    }
    location
  />;
};