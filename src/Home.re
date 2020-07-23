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
  let make = (~rows: array(string)) =>
    <View style=styles##logBox>
      {rows
       |> Array.map(log => <Text key=log> log->React.string </Text>)
       |> React.array}
    </View>;
};

let logContact = (contact: Contacts.contact) =>
  contact.givenName
  ++ " ph: "
  ++ contact.phoneNumbers
     ->Belt.Array.reduce("", (acc, ph) => ph.number ++ " | ");

module MapBtn = {
  [@react.component]
  let make = (~navigation, ~title) => {
    let handleViewMapClick = _e =>
      navigation->AppStack.Navigation.navigate("Map");

    <Button title onPress=handleViewMapClick />;
  };
};

[@react.component]
let make = (~navigation, ~logger: Hooks.logger, ~location) => {
  let (viewLogsTapCount, setViewLogsTapCount) = React.useState(() => 1);

  let report = (text, debugInfo) => {
    logger.log(text ++ (debugInfo != "" ? " : " ++ debugInfo : ""));
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

  let handleAddContactClick = _e =>
    Contacts.openContactForm(Contacts.createContact(), res =>
      switch (res, location) {
      | (Ok(Some(contact)), Hooks.Ok((lat, lon))) =>
        Contacts.addContact(
          {
            ...contact,
            Contacts.postalAddresses: [|
              {
                label: "added at",
                formattedAddress: "",

                street: lat->string_of_float ++ ", " ++ lon->string_of_float,
                pobox: "",
                neighborhood: "",
                city: "",
                region: "",
                state: "",
                postCode: "",
                country: "",
              },
            |],
          },
          res =>
          switch (res) {
          | Ok(c) => report("Successfully saved contact", logContact(c)) /* successfully saved address */
          | Error(e) => report("Contact saved without location", e) /* saving address failed */
          }
        ) /*successfully saved contact */
      | (Ok(Some(c)), _) =>
        report("Contact saved without location", logContact(c)) /*save the contact but warn about address not saved */
      | (Ok(None), _) => report("Adding contact cancelled", "") /*save the contact but warn about address not saved */
      | (Error(e), _) => report("Contact was not saved", e) /*coudln't save contact TODO SHOW ERROR */
      }
    );

  let handleViewLogsPress = _e => setViewLogsTapCount(c => c + 1);

  <>
    <StatusBar
      barStyle=`darkContent
      showHideTransition=`slide
      backgroundColor="white"
    />
    <SafeAreaView style=styles##safeAreaView>
      <View style=styles##topBox>
        <View style=styles##btn>
          <Button title="Add Contact" onPress=handleAddContactClick />
        </View>
        <View style=styles##btn>
          <Button title="View Contacts" onPress=handleViewContactsClick />
        </View>
        <View style=styles##btn> <MapBtn title="View Map" navigation /> </View>
      </View>
      <View style=styles##logContainer>
        <Text onPress=handleViewLogsPress style=styles##secretButton>
          "........................."->React.string
        </Text>
        {viewLogsTapCount mod 5 == 0 ? <Log rows={logger.logs} /> : React.null}
      </View>
    </SafeAreaView>
  </>;
};