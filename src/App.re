open ReactNative;
open Belt.Result;

let styles =
  Style.(
    StyleSheet.create({
      "btn": style(~padding=20.->dp, ()),
      "safeAreaView":
        style(
          ~flexDirection=`column,
          ~display=`flex,
          ~flexGrow=1.,
          ~justifyContent=`spaceBetween,
          (),
        ),
      "topBox": style(~flex=1., ()),
      "logContainer":
        style(~alignSelf=`flexEnd, ~height=140.->dp, ~width=100.->pct, ()),
      "viewLogsBtn": style(~height=80.->dp, ~width=100.->pct, ()),
      "secretButton": style(~textAlign=`center, ~color="#ddd", ()),
    })
  );

module Log = {
  let styles =
    Style.(
      StyleSheet.create({
        "logBox":
          style(
            ~height=100.->pct,
            ~width=100.->pct,
            ~overflow=`scroll,
            ~borderStyle=`dotted,
            ~borderWidth=1.,
            ~borderColor="black",
            (),
          ),
      })
    );

  [@react.component]
  let make = (~rows: array(string)) => {
    <View style=styles##logBox>
      {rows
       |> Array.map(log => <Text key=log> log->React.string </Text>)
       |> React.array}
    </View>;
  };
};

let formatTime =
  Js.Date.(
    time => {
      let fmt = f => time |> f |> int_of_float;
      let twoDigStrOfInt = i => {
        let str = string_of_int(i);
        if (str->String.length == 1) {
          "0" ++ str;
        } else {
          str;
        };
      };
      (fmt(getDate) |> (i => i + 1) |> twoDigStrOfInt)
      ++ "/"
      ++ (fmt(getMonth) |> (i => i + 1) |> twoDigStrOfInt)
      ++ " | "
      ++ (fmt(getHours) |> twoDigStrOfInt)
      ++ ":"
      ++ (fmt(getMinutes) |> twoDigStrOfInt)
      ++ "."
      ++ (fmt(getSeconds) |> twoDigStrOfInt);
    }
  );

let logContact = (contact: Contacts.contact) =>
  contact.givenName
  ++ " ph: "
  ++ contact.phoneNumbers
     ->Belt.Array.reduce("", (acc, ph) => ph.number ++ " | ");

[@react.component]
let app = () => {
  module Js = Js;
  let (location, setLocation) = React.useState(() => None);
  let (viewLogsTapCount, setViewLogsTapCount) = React.useState(() => 1);
  let (ready, setReady) = React.useState(() => None);
  let (logs, log) = React.useState(() => [||]);
  let log = (str: string) =>
    log(currentLogs =>
      currentLogs
      |> Array.append([|
           "> " ++ formatTime(Js.Date.(now() |> fromFloat)) ++ " ... " ++ str,
         |])
    );

  React.useEffect0(() =>
    PermissionsAndroid.(
      {
        requestMultiple([|
          Permission.readContacts,
          Permission.writeContacts,
          Permission.accessFineLocation,
        |])
        ->Promise.Js.fromBsPromise
        ->Promise.Js.toResult
        ->Promise.map(res =>
            switch (res) {
            | Ok(res) =>
              switch (
                res->get(Permission.readContacts),
                res->get(Permission.writeContacts),
                res->get(Permission.accessFineLocation),
              ) {
              | (Some(rcontacts), Some(wcontacts), Some(location))
                  when
                    rcontacts == Result.granted
                    && wcontacts == Result.granted
                    && location == Result.granted =>
                setReady(s => Some(true));
                Ok();
              | _ =>
                setReady(s => Some(false));
                Error();
              }
            | Error(_) =>
              setReady(s => Some(false));
              Error();
            }
          )
        ->Promise.getOk(_ => {
            Js.log(5555);
            Contacts.getAll(a => Js.log(a));
            let location =
              Geolocation.getCurrentPosition(
                pos =>
                  Geocoder.reverseGeocodeStreet(
                    pos##coords##latitude,
                    pos##coords##longitude,
                  )
                  ->Promise.get(res => {
                      switch (res) {
                      | Ok(_) => log("geonames location has been set")
                      | Error(e) =>
                        log("unable to get geonames location: " ++ e)
                      };

                      setLocation(l => Some(res));
                    }),
                ~onError=
                  e =>
                    log(
                      "An error occured getting geolocation: " ++ e##message,
                    ),
                ~options=
                  Geolocation.currentPositionOptions(
                    ~enableHighAccuracy=false,
                    ~timeout=20000.,
                    ~maximumAge=10000.,
                    (),
                  ),
                (),
              );
            ();
          });
        /* if (status == PermissionsAndroid.Result.granted) {
             setReady(s
               => Some(true));
               /* Contacts.
                  Contacts.getAll(d => {
                    Js.log(d);
                    setContacts(s => Some(d));
                  }); */
           } else {

           } */
        None;
      }
    )
  );
  let report = (text, debugInfo) => {
    log(text ++ (debugInfo != "" ? " : " ++ debugInfo : ""));
    Snackbar.show({
      Snackbar.text,
      Snackbar.duration: Snackbar.obj.lengthLong,
    });
  };

  let rec openContactPicker = _ =>
    Contacts.openContactPicker(res =>
      switch (res) {
      | Ok(Some(contact)) =>
        Contacts.viewExistingContact(contact, _ => openContactPicker())
      | _ => ()
      }
    );

  let handleViewContactsClick = _e => openContactPicker();

  let handleAddContactClick = _e => {
    Contacts.openContactForm(Contacts.createContact(), res =>
      switch (res, location) {
      | (Ok(Some(contact)), Some(Ok(loc))) =>
        // Contacts.deleteContact(contact);
        Contacts.addContact(
          {
            ...contact,
            Contacts.postalAddresses: [|
              {
                label: "added at",
                formattedAddress: "",

                street:
                  loc.intersection.street1
                  ++ ", "
                  ++ loc.intersection.cityName
                  ++ ", "
                  ++ loc.intersection.adminName1
                  ++ ", "
                  ++ loc.intersection.countryCode,
                pobox: "",
                neighborhood: "",
                city: "", //loc.intersection.cityName,
                region: "", //loc.intersection.adminName1,
                state: "",
                postCode: "",
                country: "" //loc.intersection.countryCode,
              },
            |],
          },
          res =>
          switch (res) {
          | Ok(c) => report("Successfully saved contact", logContact(c)) // successfully saved address
          | Error(e) => report("Contact saved without location", e) // saving address failed
          }
        ) //successfully saved contact
      | (Ok(Some(c)), _) =>
        report("Contact saved without location", logContact(c)) //save the contact but warn about address not saved
      | (Ok(None), _) => report("Adding contact cancelled", "") //save the contact but warn about address not saved
      | (Error(e), _) => report("Contact was not saved", e) //coudln't save contact TODO SHOW ERROR
      }
    );
  };

  let handleViewLogsPress = _e => {
    setViewLogsTapCount(c => c + 1);
  };

  <>
    <StatusBar
      barStyle=`darkContent
      showHideTransition=`slide
      backgroundColor="white"
    />
    {switch (ready) {
     | Some(true) =>
       <SafeAreaView style={styles##safeAreaView}>
         <View style={styles##topBox}>
           <View style=styles##btn>
             <Button title="Add Contact" onPress=handleAddContactClick />
           </View>
           <View style=styles##btn>
             <Button title="View Contacts" onPress=handleViewContactsClick />
           </View>
         </View>
         <View style={styles##logContainer}>
           <Text onPress=handleViewLogsPress style={styles##secretButton}>
             "........................."->React.string
           </Text>
           {viewLogsTapCount mod 5 == 0 ? <Log rows=logs /> : React.null}
         </View>
       </SafeAreaView>
     | Some(false) => <Text> "Permissions not granted"->React.string </Text>
     | None => React.null
     }}
  </>;
};