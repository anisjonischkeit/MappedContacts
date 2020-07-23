open ReactNative;
open ReactNativeMaps;

let regionDeltas =
  ReactNativeMaps.Region.create(
    ~latitudeDelta=0.0922,
    ~longitudeDelta=0.0421,
  );

let styles = StyleSheet.create({"map": StyleSheet.absoluteFillObject});

type marker = {
  latitude: float,
  longitude: float,
  title: string,
};

[@react.component]
let make = (~markers, ~location) => {
  let region =
    switch (location) {
    | Hooks.Ok((lat, lon)) => regionDeltas(~latitude=lat, ~longitude=lon)
    | _ => regionDeltas(~latitude=37.78825, ~longitude=-122.4324)
    };

  <MapView region style={styles##map}>
    {markers
     |> Array.map(marker => {
          <Marker
            coordinate={ReactNativeMaps.LatLng.create(
              marker.latitude,
              marker.longitude,
            )}
            title={marker.title}
          />
        })
     |> React.array}
  </MapView>;
};