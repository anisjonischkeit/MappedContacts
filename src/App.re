open ReactNative;

module ModalScreen = {
  open ReactNative;
  [@react.component]
  let make = (~navigation, ~route) =>
    <Text> {j|Hello From Modal|j}->React.string </Text>;
};

module RootStackScreen = {
  let make = () =>
    <ReactNavigation.Native.NavigationNativeContainer>
      <AppStack.Navigator mode=`modal headerMode=`none>
        <AppStack.Screen name="Main" component=Home.make />
        <AppStack.Screen name="Map" component=Map.make />
      </AppStack.Navigator>
    </ReactNavigation.Native.NavigationNativeContainer>;
};

let app = RootStackScreen.make;