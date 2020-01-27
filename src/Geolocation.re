type coords = {
  .
  "speed": float,
  "longitude": float,
  "latitude": float,
  "accuracy": float,
  "heading": float,
};

type position = {
  .
  "coords": coords,
  "timestamp": float,
};

type error = {
  .
  "code": int,
  "message": string,
};

type currentPositionOptions;
[@bs.obj]
external currentPositionOptions:
  (
    ~timeout: float=?,
    ~maximumAge: float=?,
    ~enableHighAccuracy: bool=?,
    unit
  ) =>
  currentPositionOptions =
  "";

[@bs.module "@react-native-community/geolocation"]
external getCurrentPosition:
  (
    position => unit,
    ~onError: error => unit=?,
    ~options: currentPositionOptions=?,
    unit
  ) =>
  unit =
  "";