type canvas;
type context;
type rect;

[@bs.send] external getBoundingClientRect: (canvas) => rect = "getBoundingClientRect"

[@bs.get] external getleft: (rect) => int = "left"

[@bs.get] external getTop: (rect) => int = "top"

[@bs.send] external getContext : (canvas, string) => context = "getContext";

[@bs.send] external fillRect : (context, int, int, int, int) => unit = "fillRect";

[@bs.send] external clearRect : (context, int, int, int, int) => unit = "clearRect";

[@bs.send] external moveTo : (context, float, float) => unit = "moveTo"

[@bs.send] external lineTo : (context, float, float) => unit = "lineTo"

[@bs.send] external stroke: (context) => unit = "stroke"

[@bs.set] external setFillStyle : (context, string) => unit = "fillStyle";

[@bs.set] external setStrokeStyle : (context, string) => unit = "strokeStyle";

[@bs.set] external setLineWidth : (context, int) => unit = "lineWidth";

let canvasId = "game-of-life";
let cellSize = 30;
let lineColor = "#A3A3C2";
let cellColor = "#6600FF";
let lineWidth = 1;

let getElementById: string => canvas = 
[%bs.raw {|
    function(arg) {
        return document.getElementById(arg)
    }|}]

let getCanvasSize = (game: Game.game) : (int, int) => {
    let gameSize = Game.getSize(game);
    let actualSize = (size: int) : int => size * cellSize + (size + 1) * lineWidth;
    (actualSize(gameSize.width), actualSize(gameSize.height))
};

let clear = (context, game: Game.game): unit => {
    let (width, height)  = getCanvasSize(game);
    clearRect(context, 0, 0, width, height);
};    

let drawGrid = (context, game: Game.game) : unit => {
    let (width, height) = getCanvasSize(game);
    let widthF = float_of_int(width);
    let heightF = float_of_int(height);
    let lineWidthF = float_of_int(lineWidth);
    let cellSizeF = float_of_int(cellSize);

    for (i in 0 to width) {
        let i = float_of_int(i);
        // the actual line takes up space on both sides of the defined line
        moveTo(context, (i *. lineWidthF)  +. (cellSizeF *. i) +. lineWidthF /. 2.0, 0.0);
        lineTo(context, (i *. lineWidthF)  +. (cellSizeF *. i) +. lineWidthF /. 2.0, heightF);
    }
    for (i in 0 to height) {
        let i = float_of_int(i);
        moveTo(context, 0.0,  (i *. lineWidthF)  +. (cellSizeF *. i) +. lineWidthF /. 2.0);
        lineTo(context, widthF,  (i *. lineWidthF)  +. (cellSizeF *. i) +. lineWidthF /. 2.0);
    }
    setLineWidth(context, lineWidth);
    setStrokeStyle(context, lineColor);
    stroke(context);
};

let drawCells = (context, game: Game.game) : unit => {
    setFillStyle(context, cellColor);
    Game.getLivingCells(game)
    |> List.iter ( (cell: Game.cell) => {
        let xStart = (cell.x + 1) * lineWidth + (cell.x * cellSize);
        let yStart = (cell.y + 1) * lineWidth + (cell.y * cellSize);
        fillRect(context, xStart, yStart, cellSize, cellSize);
    });
};

let getCell = (x: int, y: int): option(Game.cell) => {
    let size = cellSize + lineWidth
    if (x mod size == 1 || y mod size == 1) {
        None;
    } else {
        Some({ x: x / size, y: y / size});
    }
};

[@react.component]
let make = (~game, ~onCellClick) => {
React.useEffect(() => {
    let ctx = getContext(getElementById(canvasId),"2d");
    clear(ctx, game);
    drawGrid(ctx, game);
    drawCells(ctx, game);
    None;
  });

let (width, height) = getCanvasSize(game);
let widthS = string_of_int(width)
let heightS = string_of_int(height)

let onClick = (evt) => {
    let rect = getBoundingClientRect(getElementById(canvasId));
    let canvasLeft = getleft(rect);
    let canvasTop = getTop(rect);
    let x = ReactEvent.Mouse.clientX(evt) - canvasLeft;
    let y = ReactEvent.Mouse.clientY(evt) - canvasTop;
    switch (getCell(x, y)){
    | Some(cell) => onCellClick(cell)
    | None => ()
    };
};

<div>
    <canvas id=canvasId width=widthS height=heightS onClick={evt => onClick(evt)}></canvas>
</div>;
};
