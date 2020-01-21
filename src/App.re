open ReactNative;
open Belt.Result;

module Header = {
  [@react.component] [@bs.module "react-native/Libraries/NewAppScreen"]
  external make: _ => React.element = "Header";
};

let styles =
  Style.(
    StyleSheet.create({
      "rowText":
        style(
          ~width=100.->pct,
          ~height=60.->pt,
          ~textAlignVertical=`center,
          ~fontSize=16.,
          (),
        ),
    })
  );

let defaultEmptyStr = a =>
  Js.Nullable.toOption(a)->Belt.Option.getWithDefault("");

[@react.component]
let app = () => {
  let (contacts, setContacts) = React.useState(() => None);
  React.useEffect0(() => {
    PermissionsAndroid.(request(Permission.readContacts))
    ->Promise.Js.fromBsPromise
    ->Promise.Js.toResult
    ->Promise.getOk((status: ReactNative.PermissionsAndroid.Result.t) =>
        if (status == ReactNative.PermissionsAndroid.Result.granted) {
          Contacts.getAll(d => {
            Js.log(d);
            setContacts(s => Some(d));
          });
        } else {
          setContacts(s => Some(Error("permissions not granted")));
        }
      );
    None;
  });

  let handleContactClick = (contact, ()) =>
    Contacts.viewExistingContact(contact, d => Js.log2("asdasd", d));

  <>
    <StatusBar
      barStyle=`darkContent
      showHideTransition=`slide
      backgroundColor="white"
    />
    <SafeAreaView>
      <ScrollView contentInsetAdjustmentBehavior=`automatic>
        Contacts.(
          switch (contacts) {
          | Some(Ok(c)) =>
            c
            |> Array.map(contact =>
                 <View key={contact.recordID}>
                   <Text
                     style=styles##rowText
                     onPress={handleContactClick(contact)}>
                     {(contact.givenName ++ " " ++ contact.familyName)
                      ->React.string}
                   </Text>
                 </View>
               )
            |> React.array
          | _ => <Text key="no-contacts"> "no contacts"->React.string </Text>
          }
        )
      </ScrollView>
    </SafeAreaView>
  </>;
};