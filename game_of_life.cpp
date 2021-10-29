#include <math.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// ----------------------------------------
// CONFIGURATION
// ----------------------------------------

// BOARD
const int BOARD_HEIGHT = 40;
const int BOARD_WIDTH = 40;

// TURN
const int TURNS = 50;
const float TURN_DURATION_IN_SECONDS = 0.1;

// DISPLAY
const string DEAD_CELL_SYMBOL = "\u25A1";
const string ALIVE_CELL_SYMBOL = "\u25A0";

// ALTERNATIVE DISPLAY
// const string DEAD_CELL_SYMBOL = " ";
// const string ALIVE_CELL_SYMBOL = "*";

// INTERNAL
const string CONFIG_FILENAME = "./config.txt";
const float TURN_DURATION = TURN_DURATION_IN_SECONDS * 1000 * 1000;

// ----------------------------------------
// ----------------------------------------
// ----------------------------------------

// get config file height, width and check for errors
int* get_config_state(int max_sy, int max_sx) {
  bool has_error = false;
  bool was_set = false;
  int sy = 0;
  int sx = 0;
  int choice;

  ifstream Config(CONFIG_FILENAME);

  if (Config.good()) {
    string line;

    while (getline(Config, line)) {
      if (!was_set) {
        was_set = true;
        sx = line.length();
      } else if (sx != line.length()) {
        cout << "ERROR: Columns count is not equal on all rows." << endl;

        has_error = true;
        break;
      }

      sy++;
    }

    if (sy > max_sy) {
      has_error = true;
      cout << "ERROR: Config file rows count is exceding board limits." << endl
           << "HELP: Increase board height or decrase config file rows"
              "count."
           << endl;
    }

    if (sx > max_sx) {
      has_error = true;
      cout << "ERROR: Config file columns count is exceding board limits."
           << endl
           << "HELP: Increase board width or decrase config file"
              "columns count."
           << endl;
    }

  } else {
    has_error = true;

    cout << "ERROR: Config file named '" << CONFIG_FILENAME << "' not found."
         << endl
         << "ACTION: Would you like to create basic config file for you?"
         << endl
         << "[1] CREATE" << endl
         << "[0] EXIT" << endl;

    while (choice != 0 && choice != 1) {
      cin >> choice;
    }
  }

  Config.close();

  if (has_error && choice == 1) {
    ofstream BasicConfig(CONFIG_FILENAME);
    BasicConfig << "101" << endl << "111" << endl << "010";
    BasicConfig.close();

    return get_config_state(max_sy, max_sx);
  }

  int* ptr = new int[3]{sy, sx, int(has_error)};
  return ptr;
}

// initialize board with configuration part in the middle of the board
int** init_board(int sy, int sx, int config_sy, int config_sx) {
  int** board;
  board = new int*[sy];
  for (int i = 0; i < sy; i++) {
    board[i] = new int[sx];

    for (int j = 0; j < sx; j++) {
      board[i][j] = 0;
    }
  }

  ifstream Config(CONFIG_FILENAME);

  int board_start_y = floor((sy - 1) / 2) - floor((config_sy - 1) / 2);
  int board_start_x = floor((sx - 1) / 2) - floor((config_sx - 1) / 2);

  for (int i = board_start_y; i < board_start_y + config_sy; i++) {
    string line;
    getline(Config, line);

    for (int j = board_start_x; j < board_start_x + config_sx; j++) {
      board[i][j] = atoi(line.substr(j - board_start_x, 1).c_str());
    }
  }

  Config.close();

  return board;
}

// render graphical representation of single cell
string render_cell(int cell) {
  if (cell == 0) {
    return DEAD_CELL_SYMBOL;
  } else {
    return ALIVE_CELL_SYMBOL;
  }
}

// render whole board based on given input
void render_board(int** arr, int sy, int sx) {
  system("clear");

  for (int i = 0; i < sy; i++) {
    for (int j = 0; j < sx; j++) {
      cout << render_cell(arr[i][j]) << " ";
    }

    cout << endl;
  }
}

// ger sum of all alive neighbors of the cell
int get_alive_neighbors(int** arr, int row, int col, int sy, int sx) {
  int n_sum = 0;

  int upper_row = row - 1;
  int lower_row = row + 1;
  int right_col = col + 1;
  int left_col = col - 1;

  bool possible_upper_row = upper_row >= 0;
  bool possible_lower_row = lower_row < sy;
  bool possible_right_col = right_col < sx;
  bool possible_left_col = left_col >= 0;

  // N
  if (possible_upper_row) {
    n_sum += arr[upper_row][col];
  }

  // NE
  if (possible_upper_row && possible_right_col) {
    n_sum += arr[upper_row][right_col];
  }

  // E
  if (possible_right_col) {
    n_sum += arr[row][right_col];
  }

  // SE
  if (possible_lower_row && possible_right_col) {
    n_sum += arr[lower_row][right_col];
  }

  // S
  if (possible_lower_row) {
    n_sum += arr[lower_row][col];
  }

  // SW
  if (possible_lower_row && possible_left_col) {
    n_sum += arr[lower_row][left_col];
  }

  // W
  if (possible_left_col) {
    n_sum += arr[row][left_col];
  }

  // NW
  if (possible_upper_row && possible_left_col) {
    n_sum += arr[upper_row][left_col];
  }

  return n_sum;
}

// make one turn
void make_turn(int** arr, int sy, int sx) {
  int** temp_arr;
  temp_arr = new int*[sy];
  for (int i = 0; i < sy; i++) {
    temp_arr[i] = new int[sx];
  }

  // copy array
  for (int i = 0; i < sy; i++) {
    for (int j = 0; j < sx; j++) {
      temp_arr[i][j] = arr[i][j];
    }
  }

  for (int i = 0; i < sy; i++) {
    for (int j = 0; j < sx; j++) {
      int cell = arr[i][j];
      int neighbors = get_alive_neighbors(temp_arr, i, j, sy, sx);

      if (cell == 1) {
        if (neighbors != 2 && neighbors != 3) {
          arr[i][j] = 0;
        }
      } else {
        if (neighbors == 3) {
          arr[i][j] = 1;
        }
      }
    }
  }

  // temp array cleanup
  for (int i = 0; i < sy; i++) {
    delete[] temp_arr[i];
  }

  delete[] temp_arr;
}

// start the game on given board
int** start(int** arr, int sy, int sx) {
  for (int i = 0; i < TURNS; i++) {
    render_board(arr, sy, sx);
    cout.flush();

    make_turn(arr, sy, sx);
    usleep(TURN_DURATION);
  }

  return arr;
}

int main() {
  // setup part
  // ----------------------------------------

  int sy = BOARD_HEIGHT;
  int sx = BOARD_WIDTH;

  int* state = get_config_state(sy, sx);
  int config_sy = state[0];
  int config_sx = state[1];
  int config_has_error = state[2];

  if (config_has_error) {
    return 0;
  }

  // menu part
  // ----------------------------------------

  int choice;
  int** current_board;
  bool use_current_board = false;

  cout << "INFO: Config file loaded successfully." << endl
       << "ACTION: Do you want to start?" << endl
       << "[1] START" << endl
       << "[0] EXIT" << endl;

  while (choice != 0 && choice != 1) {
    cin >> choice;
  }

  while (choice == 1 || choice == 2) {
    int** board;
    choice = -1;

    if (use_current_board) {
      board = current_board;
    } else {
      board = init_board(sy, sx, config_sy, config_sx);
    }

    current_board = start(board, sy, sx);

    cout << "INFO: Program execution finished." << endl
         << "ACTION: Do you want to continue?" << endl
         << "[1] CONTINUE" << endl
         << "[2] RESTART" << endl
         << "[0] EXIT" << endl;

    while (choice != 0 && choice != 1 && choice != 2) {
      cin >> choice;
    }

    if (choice == 1) {
      use_current_board = true;
    } else {
      use_current_board = false;
    }
  }
}