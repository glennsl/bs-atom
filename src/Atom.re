type compositeDisposable;
type cursor;
type disposable;
type panel;
type textEditor;
type view;

module CompositeDisposable = {
  type t = compositeDisposable;

  external make : unit => t = "CompositeDisposable" [@@bs.new] [@@bs.module "atom"];

  external add : t => disposable => unit = "" [@@bs.send];
  external destroy : t => unit = "" [@@bs.send];
};

/* CommandRegistry */
module Commands = {
  external add : string => Js.t {..} => disposable = "atom.commands.add" [@@bs.val];
  external addToElement : Dom.element => string => (Dom.event => unit) => disposable = "atom.commands.add" [@@bs.val];
};

module Cursor = {
  type t = cursor;

  external getScreenRow : int = "" [@@bs.send.pipe: t];
  external moveUp : unit = "" [@@bs.send.pipe: t];
  external moveUpBy : int => unit = "" [@@bs.send.pipe: t];
  external moveDown : unit = "" [@@bs.send.pipe: t];
  external moveDownBy : int => unit = "" [@@bs.send.pipe: t];
  external moveLeft : unit = "" [@@bs.send.pipe: t];
  external moveLeftBy : int => unit = "" [@@bs.send.pipe: t];
  external moveRight : unit = "" [@@bs.send.pipe: t];
  external moveRightBy : int => unit = "" [@@bs.send.pipe: t];
};

module Panel = {
  type t = panel;

  external isVisible : bool = "" [@@bs.send.pipe: t];
  external hide : unit = "" [@@bs.send.pipe: t];
  external show : unit = "" [@@bs.send.pipe: t];
  external destroy : unit = "" [@@bs.send.pipe: t];
};

module TextEditor = {
  type t = textEditor;

  external getCursors : array cursor = "" [@@bs.send.pipe: t];
  external isMini : bool = "" [@@bs.send.pipe: t];

  /* extension */
  external getView : t => Dom.element = "atom.views.getView" [@@bs.val];
};

/* ViewRegistry */
module Views = {
  external getView : 'a => Dom.element = "atom.views.getView" [@@bs.val]; /* throws */
};

module Workspace = {
  external addModalPanel : Js.t {..} => panel = "atom.workspace.addModalPanel" [@@bs.val];
  external observeTextEditors : (textEditor => unit) => disposable = "atom.workspace.observeTextEditors" [@@bs.val];
};

module type HooksSpec = {
  type state;
  type serializedState;

  let activate : option serializedState => state;
  let serialize : state => serializedState;
  let deactivate : state => unit;
};

module Hooks (Spec: HooksSpec) => {
  type self = {
    mutable state: Spec.state
  };

  let activate = (fun self serializedState => {
    self.state = Spec.activate (Js.Undefined.to_opt serializedState);
  }) [@bs.this];

  let serialize = (fun self => Spec.serialize self.state) [@bs.this];

  let deactivate = (fun self => Spec.deactivate self.state) [@bs.this];
};
