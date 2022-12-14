//ALL TABLES ARE CALCULATED FROM WITH POV`

const int PAWN_VALUE_TABLE[64] = {  0,   0,   0,   0,   0,   0,   0,   0,
                                   50,  50,  50,  50,  50,  50,  50,  50,
                                   10,  10,  20,  30,  30,  20,  10,  10,
                                    5,   5,  10,  25,  25,  10,   5,   5,
                                    0,   0,   0,  20,  20,   0,   0,   0,
                                    5,  -5, -10,   0,   0, -10,  -5,   5,
                                    0,   0,   0,   0,   0,   0,   0,   0
                                };

const int KNIGHT_VALUE_TABLE[64] = {-50, -40, -30, -30, -30, -30, -40, -50,
                                  -40, -20,   0,   0,   0,   0, -20, -40,
                                  -30,   0,  10,  15,  15,  10,   0, -30,
                                  -30,   5,  15,  20,  20,  15,   5, -30,
                                  -30,   0,  15,  20,  20,  15,   0, -30,
                                  -30,   5,  10,  15,  15,  10,   5, -30,
                                  -40, -20,   0,   5,   5,   0, -20, -40,
                                  -50, -40, -30, -30, -30, -30, -40, -50
                              };

const int BISHOP_VALUE_TABLE[64] = {-20, -10, -10, -10, -10, -10, -10, -20,
                                    -10,   0,   0,   0,   0,   0,   0, -10,
                                    -10,   0,   5,  10,  10,   5,   0, -10,
                                    -10,   5,   5,  10,  10,   5,   5, -10,
                                    -10,   0,  10,  10,  10,  10,   0, -10,
                                    -10,  10,  10,  10,  10,  10,  10, -10,
                                    -10,   5,   0,   0,   0,   0,   5, -10,
                                    -20, -10, -10, -10, -10, -10, -10, -20
                                };

const int ROOK_VALUE_TABLE[64] = {   0,   0,   0,   0,   0,   0,   0,   0,
                                     5,  10,  10,  10,  10,  10,  10,   5,
                                    -5,   0,   0,   0,   0,   0,   0,  -5,
                                    -5,   0,   0,   0,   0,   0,   0,  -5,
                                    -5,   0,   0,   0,   0,   0,   0,  -5,
                                    -5,   0,   0,   0,   0,   0,   0,  -5,
                                    -5,   0,   0,   0,   0,   0,   0,  -5,
                                     0,   0,   0,   5,   5,   0,   0,   0
                                };

const int QUEEN_VALUE_TABLE[64] = {-20, -10, -10,  -5,  -4, -10, -10, -20,
                                   -10,   0,   0,   0,   0,   0,   0, -10,
                                   -10,   0,   5,   5,   5,   5,   0, -10,
                                  - 5,   0,   5,   5,   5,   5,   0,  -5,
                                     0,   0,   5,   5,   5,   5,   0,  -5,
                                   -10,   5,   5,   5,   5,   5,   0, -10,
                                   -10,   0,   5,   0,   0,   0,   0, -10,
                                   -20, -10, -10,  -5,  -5, -10, -10, -20
                                };

const int KING_VALUE_TABLE[64] = {-30, -40, -40, -50, -50, -40, -40, -30,
                                  -30, -40, -40, -50, -50, -40, -40, -30,
                                  -30, -40, -40, -50, -50, -40, -40, -30,
                                  -30, -40, -40, -50, -50, -40, -40, -30,
                                  -20, -30, -30, -40, -40, -30, -30, -20,
                                  -10, -20, -20, -20, -20, -20, -20, -10,
                                  20,  20,   0,   0,   0,   0,  20,  20,
                                  20,  30,  10,   0,   0,  10,  30,  20
                                };

const int KING_VALUE_TABLE_END[64] = {-50, -40, -30, -20, -20, -30, -40, -50,
                                      -30, -20, -10,   0,   0, -10, -20, -30,
                                      -30, -10,  20,  30,  30,  20, -10, -30,
                                      -30, -10,  30,  40,  40,  30, -10, -30,
                                      -30, -10,  30,  40,  40,  30, -10, -30,
                                      -30, -10,  20,  30,  30,  20, -10, -30,
                                      -30, -30,   0,   0,   0,   0, -30, -30,
                                      -50, -30, -30, -30, -30, -30, -30, -50
                                    };

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;
const int KING_VALUE = 20000;