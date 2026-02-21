# DrawboardCLI

## Console-based application which represents a shapes board with the ability to draw different types of shapes on it.

# Available commands:

| **Command** | **Explanation** |
| --- | --- |
| `help` | Prints available commands. |
| `exit` | Exits program. |
| `create` | Create blackboard. |
| `draw` | Draw blackboard to the console. |
| `color` | Add custom color. |
| `colors` | List all available colors. |
| `shape` | Print a list of all available shapes and parameters for add call. |
| `add <params>` | Add shape with specified colour and fill mode to the blackboard. |
| `select <id\|x y>` | Select either a shape with an ID or foreground shape by the selected coordinates. |
| `remove` | Remove selected from blackboard the shape. |
| `edit <params>` | Modify the parameters of the selected figure. |
| `paint <color>`   | Change the colour the selected of figure. |
| `move <x y>` | Move the selected shape to the coordinates. |
| `clear` | Remove all shapes from blackboard. |
| `save <filename>`  | Save the blackboard to the file. |
| `load <filename>` | Load the blackboard from the file. |
