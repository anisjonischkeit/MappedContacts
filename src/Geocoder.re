type intersection = {
  adminCode1: string,
  placename: string,
  adminName2: string,
  postalcode: string,
  countryCode: string,
  street1: string,
  street2: string,
  adminName1: string,
  cityName: string,
};
type result = {intersection};

open Belt.Result;
let reverseGeocodeStreet = (lat, lon) => {
  Js.log(
    "http://api.geonames.org/findNearestIntersectionOSMJSON?username=anis&lat="
    ++ lat->string_of_float
    ++ "&lng="
    ++ lon->string_of_float
    ++ "&includeGeoName=true",
  );

  (
    Fetch.fetch(
      "http://api.geonames.org/findNearestIntersectionOSMJSON?username=anis&lat="
      ++ lat->string_of_float
      ++ "&lng="
      ++ lon->string_of_float
      ++ "&includeGeoName=true",
    )
    |> Js.Promise.then_(Fetch.Response.json)
  )
  ->Promise.Js.fromBsPromise
  ->Promise.Js.toResult
  ->Promise.map(data => {
      Js.log("data");
      switch (data) {
      | Ok(data) =>
        Js.log(data);
        Ok(data->Obj.magic: result);
      | Error(e) =>
        Error("Something went wrong reverse geocoding: " ++ e->Js.String.make)
      };
    });
};