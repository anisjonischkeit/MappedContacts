open ReactNative;

[@react.component]
let make = (~navigation, ~route, ~location) => {
  <Map markers=[||] location />;
};