#include "epdgui_checkerboard.h"

void EPDGUI_Checkerboard::Randomize()
{
    uint16_t center_x = BOARD_WIDTH / 2;
    uint16_t center_y = BOARD_HEIGHT / 2;
    uint16_t half_cell_count = GO_CELL_COUNT / 2;

    uint8_t randbuf[GO_CELL_COUNT * GO_CELL_COUNT];
    esp_fill_random(randbuf, GO_CELL_COUNT * GO_CELL_COUNT);
    for (int y = 0; y < GO_CELL_COUNT; y++)
    {
        int y_diff = y - half_cell_count;
        for (int x = 0; x < GO_CELL_COUNT; x++)
        {
            int x_diff = x - half_cell_count;
            uint8_t value = randbuf[x * GO_CELL_COUNT + y];
            if (value & 0x01)
            {
                // 一半概率画上，一半概率不画
                if (value & 0x02)
                {
                    // 一半概率白棋子
                    DrawBlackPiece(center_x + x_diff * GO_CELL_SIZE, center_y + y_diff * GO_CELL_SIZE);
                }
                else
                {
                    // 一半概率黑棋子
                    DrawWhitePiece(center_x + x_diff * GO_CELL_SIZE, center_y + y_diff * GO_CELL_SIZE);
                }
            }
        }
    }
}

void EPDGUI_Checkerboard::DrawBlackPiece(uint32_t x, uint32_t y)
{
    // 画黑棋子
    _canvas->fillCircle(x, y, PIECES_R, 15);
}

void EPDGUI_Checkerboard::DrawWhitePiece(uint32_t x, uint32_t y)
{
    // 画白棋子
    _canvas->fillCircle(x, y, PIECES_R, 0);
    _canvas->drawCircle(x, y, PIECES_R, 15);
}

void EPDGUI_Checkerboard::UpdateTouchedPiece(int16_t x, int16_t y)
{
    uint32_t center_x = BOARD_WIDTH / 2;
    uint32_t center_y = BOARD_HEIGHT / 2 + BOARD_MARGIN_TOP;
    uint32_t half_count = GO_CELL_COUNT / 2;
    uint32_t half_size = GO_CELL_SIZE / 2;
    uint32_t board_left = center_x - half_count * GO_CELL_SIZE - half_size;
    uint32_t board_top = center_y - half_count * GO_CELL_SIZE - half_size;
    uint32_t board_right = center_x + half_count * GO_CELL_SIZE + half_size;
    uint32_t board_bottom = center_y + half_count * GO_CELL_SIZE + half_size;
    // log_d("%d %d %d %d", board_left, board_top, board_right, board_bottom);
    if (x >= board_left && x <= board_right && y >= board_top && y <= board_bottom)
    {
        _prev_touched_x = _touched_x;
        _prev_touched_y = _touched_y;
        _touched_x = (x - board_left) / GO_CELL_SIZE;
        _touched_y = (y - board_top) / GO_CELL_SIZE;
        if (_prev_touched_x != _touched_x || _prev_touched_y != _touched_y)
        {
            // log_d("x,y = %d,%d", _touched_x, _touched_y);
            _is_dirty = true;
            _last_touched_time = millis();
        }
        else
        {
            auto now = millis();
            if (now - _last_touched_time > 100)
            {
                // same and real write the piece
                _pending_piece_to_write_x = _touched_x;
                _pending_piece_to_write_y = _touched_y;
                _is_dirty = true;
            }
        }
    }
}

void EPDGUI_Checkerboard::DrawTouchedPiece(uint32_t touched_x, uint32_t touched_y, enum DrawPieceType type)
{
    log_d("%d %d is_clear=%d", touched_x, touched_y, type);
    uint32_t center_x = BOARD_WIDTH / 2;
    uint32_t center_y = BOARD_HEIGHT / 2;
    uint32_t half_count = GO_CELL_COUNT / 2;
    uint32_t half_size = GO_CELL_SIZE / 2;
    int32_t diff_x = touched_x - half_count;
    int32_t diff_y = touched_y - half_count;
    uint32_t touched_center_x = center_x + diff_x * GO_CELL_SIZE;
    uint32_t touched_center_y = center_y + diff_y * GO_CELL_SIZE;
    if (type == DrawPieceType::kDrawRealPieceCircle)
    {
        if (_pending_piece_is_black)
        {
            DrawBlackPiece(touched_center_x, touched_center_y);
        }
        else
        {
            DrawWhitePiece(touched_center_x, touched_center_y);
        }
        _pending_piece_is_black = !_pending_piece_is_black;
    }
    else
    {
        uint32_t color = type == DrawPieceType::kClearPieceRect ? 0 : 15;
        log_d("drawRect: %d %d", touched_center_x - half_size, touched_center_y - half_size);
        _canvas->drawRect(touched_center_x - half_size, touched_center_y - half_size, GO_CELL_SIZE, GO_CELL_SIZE, color);
    }
}

void EPDGUI_Checkerboard::UpdateDraw()
{
    if (_is_dirty)
    {
        if (_prev_touched_x != -1 && _prev_touched_y != -1)
        {
            DrawTouchedPiece(_prev_touched_x, _prev_touched_y, EPDGUI_Checkerboard::kClearPieceRect);
        }
        if (_touched_x != -1 && _touched_y != -1)
        {
            DrawTouchedPiece(_touched_x, _touched_y, EPDGUI_Checkerboard::kDrawPieceRect);
        }
        if (_pending_piece_to_write_x != -1 && _pending_piece_to_write_y != -1)
        {
            DrawTouchedPiece(_pending_piece_to_write_x, _pending_piece_to_write_y, EPDGUI_Checkerboard::kDrawRealPieceCircle);
            _pending_piece_to_write_x = -1;
            _pending_piece_to_write_y = -1;
        }
        _canvas->pushCanvas(0, BOARD_MARGIN_TOP, UPDATE_MODE_GC16);
        _is_dirty = false;
    }
}

void EPDGUI_Checkerboard::InitializeBoard()
{
    _canvas->fillCanvas(0);
    uint16_t center_x = BOARD_WIDTH / 2;
    uint16_t center_y = BOARD_HEIGHT / 2;
    uint16_t cell_count = GO_CELL_COUNT;
    uint16_t cell_size = GO_CELL_SIZE;

    uint16_t half_cell_count = cell_count / 2;

    // Draw vertical board lines: strong
    for (int16_t x_index = 0; x_index < cell_count; x_index++)
    {
        int16_t x_diff = x_index - half_cell_count;
        _canvas->drawFastVLine(center_x + x_diff * cell_size, center_y - half_cell_count * cell_size, (cell_count - 1) * cell_size, 15);
    }
    // Draw horizontal board lines: strong
    for (int16_t y_index = 0; y_index < cell_count; y_index++)
    {
        int16_t y_diff = y_index - half_cell_count;
        _canvas->drawFastHLine(center_x - half_cell_count * cell_size, center_y + y_diff * cell_size, (cell_count - 1) * cell_size, 15);
    }
    // Draw stars
    _canvas->fillCircle(center_x, center_y, STARS_R, 15);
    _canvas->fillCircle(center_x - _15_STAR_GAP * cell_size, center_y - _15_STAR_GAP * cell_size, STARS_R, 15);
    _canvas->fillCircle(center_x + _15_STAR_GAP * cell_size, center_y - _15_STAR_GAP * cell_size, STARS_R, 15);
    _canvas->fillCircle(center_x + _15_STAR_GAP * cell_size, center_y + _15_STAR_GAP * cell_size, STARS_R, 15);
    _canvas->fillCircle(center_x - _15_STAR_GAP * cell_size, center_y + _15_STAR_GAP * cell_size, STARS_R, 15);
    _canvas->pushCanvas(0, BOARD_MARGIN_TOP, UPDATE_MODE_GC16);
}

EPDGUI_Checkerboard::EPDGUI_Checkerboard(uint32_t board) : EPDGUI_Base(0, BOARD_MARGIN_TOP, BOARD_WIDTH, BOARD_HEIGHT)
{
    // uint8_t language = GetLanguage();
    this->_canvas = new M5EPD_Canvas(&M5.EPD);
    this->_canvas->createCanvas(BOARD_WIDTH, BOARD_HEIGHT);
    for (size_t x = 0; x < GO_CELL_COUNT; x++)
    {
        for (size_t y = 0; y < GO_CELL_COUNT; y++)
        {
            Piece &piece = _pieces[x][y];
            piece.is_black = 0;
            piece.is_checked = 0;
            piece.is_weak_checked = 0;
        }
    }
}

EPDGUI_Checkerboard::~EPDGUI_Checkerboard()
{
    this->_canvas->deleteCanvas();
    delete this->_canvas;
}

void EPDGUI_Checkerboard::Draw(m5epd_update_mode_t mode)
{
    if (_ishide)
    {
        return;
    }
    InitializeBoard();
}

void EPDGUI_Checkerboard::Draw(M5EPD_Canvas *canvas)
{
    if (_ishide)
    {
        return;
    }
    InitializeBoard();
}

void EPDGUI_Checkerboard::Bind(int16_t state, void (*func_cb)(epdgui_args_vector_t &))
{
}

void EPDGUI_Checkerboard::UpdateState(int16_t x, int16_t y)
{
    if (!_isenable)
    {
        return;
    }
    // log_d("UpdateState, sizeof()=%d", sizeof(_pieces));
    bool is_down = x != -1 && y != -1;
    if (is_down)
    {
        bool is_in_box = isInBox(x, y);
        if (is_in_box)
        {
            UpdateTouchedPiece(x, y);
        }
    }
}

void EPDGUI_Checkerboard::WriteCurrentAsBlack() {}
void EPDGUI_Checkerboard::WriteCurrentAsWhite() {}
void EPDGUI_Checkerboard::RemoveCurrent() {}