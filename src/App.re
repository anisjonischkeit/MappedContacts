open ReactNative;

module ModalScreen = {
  open ReactNative;
  [@react.component]
  let make = (~navigation, ~route) =>
    <Text> {j|Hello From Modal|j}->React.string </Text>;
};

module RootStackScreen = {
  let make = () => {
    let ready = Hooks.useReady();
    let logger = Hooks.useNewLogger();
    let location = Hooks.useLocation(~log=logger.log);

    switch (ready) {
    | Some(true) =>
      <ReactNavigation.Native.NavigationContainer>
        <AppStack.Navigator mode=`modal headerMode=`none>
          <AppStack.ScreenWithCallback name="Main">
            {(
               ({navigation, route}) =>
                 <Home navigation route logger location />
             )}
          </AppStack.ScreenWithCallback>
          <AppStack.ScreenWithCallback name="Map">
            {(({navigation, route}) => <MapPage navigation route location />)}
          </AppStack.ScreenWithCallback>
        </AppStack.Navigator>
      </ReactNavigation.Native.NavigationContainer>
    | Some(false) => <Text> "Permissions not granted"->React.string </Text>
    | None => React.null
    };
  };
};

let app = RootStackScreen.make;