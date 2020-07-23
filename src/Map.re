open ReactNative;
open ReactNativeMaps;

// type region = {
//   latitude: float,
//   longitude: float,
//   latitudeDelta: float,
//   longitudeDelta: float,
// };
// module MV = MapView.Make({type T = ReactNativeMaps.Region.t});

let regionDeltas = ReactNativeMaps.Region.create(
    ~latitudeDelta= 0.0922,
    ~longitudeDelta= 0.0421
  );

[@react.component]
let make = (~navigation, ~route) => {
  let location = Hooks.useLocation();

  let region = switch(location) {
    | Ok((lat, lon)) =>regionDeltas(
        ~latitude= lat,
        ~longitude= lon,
      );
    | _ =>regionDeltas(
        ~latitude= 37.78825,
        ~longitude= (-122.4324),
      );
  };



  <MapView region />;
};