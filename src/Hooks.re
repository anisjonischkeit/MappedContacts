type location('d, 'e) =
  | Loading
  | Ok((float, float))
  | Error(string);

let useLocation = (~log) => {
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
              e => {
                log("An error occured getting geolocation: " ++ e##message);
                setLocation(l =>
                  Error(
                    "An error occured getting geolocation: " ++ e##message,
                  )
                );
              },
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

let useReady = () => {
  let (ready, setReady) = React.useState(() => None);

  React.useEffect0(() => {
    open ReactNative.PermissionsAndroid;

    requestMultiple([|
      Permission.readContacts,
      Permission.writeContacts,
      Permission.accessFineLocation,
    |])
    ->Promise.Js.fromBsPromise
    ->Promise.Js.toResult
    ->Promise.get(res =>
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
            setReady(_s => Some(true));
            ();
          | _ =>
            setReady(_s => Some(false));
            ();
          }
        | Error(_) =>
          setReady(_s => Some(false));
          ();
        }
      );
    None;
  });

  ready;
};

type logger = {
  log: string => unit,
  logs: array(string),
};

let useNewLogger = () => {
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

  let (logs, log) = React.useState(() => [||]);
  let log = (str: string) =>
    log(currentLogs =>
      currentLogs
      |> Array.append([|
           "> " ++ formatTime(Js.Date.(now() |> fromFloat)) ++ " ... " ++ str,
         |])
    );

  {log, logs};
};