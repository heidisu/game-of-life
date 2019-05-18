
[%bs.raw {|require('./App.css')|}];

type state = {
    game: Game.game,
    isPlaying: bool
  };
  
type action =
  | Step
  | Play
  | Stop
  | Clear
  | Reset
  | ChangeWidth(string)
  | ChangeHeight(string)
  | TimerTick
  | ToggleCell(Game.cell);

let maxSize = 50;

let validateInput = (value: string, state: state, update: (state, int) => state) => {
  let newValue = 
      try(int_of_string(value)){ 
      | _ => 0};
    if (newValue > 0 && newValue <= maxSize) update(state, newValue) else state
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer((state, action) =>
  switch (action) {
  | Step => { ...state, game: Game.step(state.game)}
  | Play => { ...state, isPlaying: true}
  | Stop => { ...state, isPlaying: false}
  | Clear => { game: Game.clear(state.game), isPlaying: false }
  | Reset => { game: Game.defaultGame, isPlaying: false }
  | ChangeWidth(value) => validateInput(value, state, (state, newVal) => { ...state, game: Game.setWidth(state.game, newVal)})
  | ChangeHeight(value) => validateInput(value, state, (state, newVal) => { ...state, game: Game.setHeight(state.game, newVal)})
  | TimerTick =>  if (state.isPlaying) {...state, game: Game.step(state.game)} else state
  | ToggleCell(cell) => {...state, game: Game.toggleCell(state.game, cell)}
  }, {game: Game.defaultGame, isPlaying: false});

  React.useEffect(() => {
    let timer = Js.Global.setInterval(() => dispatch(TimerTick), 500);
    Some(() => Js.Global.clearInterval(timer))
   });

   let gameSize = Game.getSize(state.game);

  <div id="main">
    <h1>{ReasonReact.string("Conway's game of life")}</h1>
    <div className="action-row">
      <button onClick={_event => dispatch(state.isPlaying ? Stop : Play)}>
        {ReasonReact.string(state.isPlaying ? "Stop" : "Play")}
        <i className={"fas fa-" ++ (state.isPlaying? "stop" : "play") ++ " fa-lg"}></i>
      </button>
      <button onClick={_event => dispatch(Step)}>
        {ReasonReact.string("Step")}
        <i className="fas fa-step-forward fa-lg"></i>
      </button>
      <button onClick={_event => dispatch(Clear)}>
        {ReasonReact.string("Clear")}
        <i className="fas fa-eraser fa-lg"></i>
      </button>
      <button onClick={_event => dispatch(Reset)}>
      {ReasonReact.string("Reset")}
      <i className="fas fa-redo fa-lg"></i>
    </button>
    </div>
    <div className="action-row">
      <label htmlFor="width">{ReasonReact.string("Width:")}</label>
      <input 
        id="width" 
        type_="number" 
        min=1 
        max=string_of_int(maxSize) 
        value=string_of_int(gameSize.width) 
        onChange={event => dispatch(ChangeWidth(ReactEvent.Form.target(event)##value))}/>
      <label htmlFor="height">{ReasonReact.string("Height:")}</label>
      <input
        id="height" 
        type_="number" 
        min=1 
        max=string_of_int(maxSize) 
        value=string_of_int(gameSize.height) 
        onChange={event => dispatch(ChangeHeight(ReactEvent.Form.target(event)##value))}
      /> 
    </div>
    <Canvas game=state.game onCellClick={cell => dispatch(ToggleCell(cell))}/>
  </div>;
};