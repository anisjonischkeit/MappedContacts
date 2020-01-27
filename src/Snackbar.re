[@bs.module "react-native-snackbar"]
external lengthShort: string = "LENGTH_SHORT";

type showOptions = {
  text: string,
  duration: int,
};

type obj = {
  show: showOptions => unit,
  [@bs.as "LENGTH_SHORT"]
  lengthShort: int,
  [@bs.as "LENGTH_LONG"]
  lengthLong: int,
};
[@bs.module "react-native-snackbar"] external obj: obj = "default";

let show = obj.show;