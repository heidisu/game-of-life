
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
  | TimerTick
  | ToggleCell(Game.cell);

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer((state, action) =>
  switch (action) {
  | Step => { ...state, game: Game.step(state.game)}
  | Play => { ...state, isPlaying: true}
  | Stop => { ...state, isPlaying: false}
  | Clear => { game: Game.clear(state.game), isPlaying: false }
  | TimerTick =>  if (state.isPlaying) {...state, game: Game.step(state.game)} else state
  | ToggleCell(cell) => {...state, game: Game.toggleCell(state.game, cell)}
  }, {game: Game.defaultGame, isPlaying: false});

  React.useEffect(() => {
    let timer = Js.Global.setInterval(() => dispatch(TimerTick), 500);
    Some(() => Js.Global.clearInterval(timer))
   });

  <div id="main">
    <h1>{ReasonReact.string("Conway's game of life")}</h1>
    <div className="button-row">
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
    </div>
    <Canvas game=state.game onCellClick={cell => dispatch(ToggleCell(cell))}/>
  </div>;
};