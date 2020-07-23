type location('d, 'e) =
  | Loading
  | Ok((float, float))
  | Error(string);

let useLocation = () => {
  open ReactNative.PermissionsAndroid;
  let (location, setLocation) = React.useState(() => Loading);

  React.useEffect0(() => {
    request(Permission.accessFineLocation)
    ->Promise.Js.fromBsPromise
    ->Promise.Js.toResult
    ->Promise.map(res =>
        switch (res) {
        | Ok(location) when location == Result.granted => Belt.Result.Ok()
        | Error(_) => Belt.Result.Error()
        }
      )
    ->Promise.getOk(_ => {
        let location =
          Geolocation.getCurrentPosition(
            pos =>
              setLocation(l =>
                Ok((pos##coords##latitude, pos##coords##longitude))
              ),
            ~onError=
              e =>
                setLocation(l =>
                  Error(
                    "An error occured getting geolocation: " ++ e##message,
                  )
                ),
            ~options=
              Geolocation.currentPositionOptions(
                ~enableHighAccuracy=true,
                ~timeout=20000.,
                ~maximumAge=10000.,
                (),
              ),
            (),
          );
        ();
      });
    None;
  });

  location;
};