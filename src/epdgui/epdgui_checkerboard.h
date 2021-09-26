#ifndef __EPDGUI_CHECKERBOARD_H
#define __EPDGUI_CHECKERBOARD_H

#include "epdgui_button.h"
#include "epdgui_switch.h"

struct PiecePosition
{
    uint16_t x;
    uint16_t y;
};

struct Piece
{
    uint8_t is_black : 1;        // 1 bit width
    uint8_t is_checked : 1;      // 1 bit width
    uint8_t is_weak_checked : 1; // 1 bit width
};

class EPDGUI_Checkerboard : public EPDGUI_Base
{
public:
    enum DrawPieceType
    {
        kDrawPieceRect = 0,
        kClearPieceRect,
        kDrawRealPieceCircle,
    };
    static const uint32_t GO_BOARD = 0x00000001;
    static const uint32_t FIR_BOARD = 0x00000002;

    static const uint32_t BOARD_MARGIN_TOP = 72 + 110;
    static const uint32_t BOARD_WIDTH = 540;
    static const uint32_t BOARD_HEIGHT = 540;

    static const uint32_t _15_STAR_GAP = 4;

    // 围棋格子尺寸
    static const uint32_t GO_CELL_SIZE = 35;
    // 围棋棋盘线数
    static const uint32_t GO_CELL_COUNT = 15;

    // 五子棋格子尺寸
    static const uint32_t FIR_CELL_SIZE = 36;
    // 五子棋棋盘线数
    static const uint32_t FIR_CELL_COUNT = 15;

    // 星位半径
    static const uint32_t STARS_R = 4;
    // 棋子半径
    static const uint32_t PIECES_R = 16;

public:
    EPDGUI_Checkerboard(uint32_t board = GO_BOARD);
    ~EPDGUI_Checkerboard();
    void Draw(m5epd_update_mode_t mode = UPDATE_MODE_DU4);
    void Draw(M5EPD_Canvas *canvas);
    void InitializeBoard();
    void Randomize();
    void Bind(int16_t event, void (*func_cb)(epdgui_args_vector_t &));
    void UpdateState(int16_t x, int16_t y);
    void UpdateTouchedPiece(int16_t x, int16_t y);
    void UpdateDraw();
    void WriteCurrentAsBlack();
    void WriteCurrentAsWhite();
    void RemoveCurrent();

private:
    void DrawTouchedPiece(uint32_t touched_x, uint32_t touched_y, enum DrawPieceType type);
    void DrawBlackPiece(uint32_t x, uint32_t y);
    void DrawWhitePiece(uint32_t x, uint32_t y);

    bool _is_dirty = false;
    long _last_touched_time = 0;
    uint32_t _prev_touched_x = -1;
    uint32_t _prev_touched_y = -1;
    uint32_t _touched_x = -1;
    uint32_t _touched_y = -1;
    uint32_t _pending_piece_to_write_x = -1;
    uint32_t _pending_piece_to_write_y = -1;
    bool _pending_piece_is_black = true;
    Piece _pieces[GO_CELL_COUNT][GO_CELL_COUNT];
    uint8_t _layout = 0;

public:
    M5EPD_Canvas *_canvas = NULL;
};

#endif //__EPDGUI_CHECKERBOARD_H