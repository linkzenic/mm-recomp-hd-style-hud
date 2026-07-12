#include "modding.h"
#include "global.h"
#include "action_label_textures.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

void Interface_SetPerspectiveView(PlayState* play, s32 topY, s32 bottomY, s32 leftX, s32 rightX);
RECOMP_IMPORT("*", u32 recomp_get_config_u32(const char* key));

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonColours_Register(s16 (**pointer)[4][3]));
static s16 (*pButtonColours)[4][3];

RECOMP_IMPORT("mm_recomp_interface_helper", void StartButtonColour_Register(s16 (**pointer)[3]));
static s16 (*pStartButtonColour)[3];

RECOMP_IMPORT("mm_recomp_interface_helper", void CUpButtonColours_Register(s16 (**pointer)[3]));
static s16 (*pCUpButtonColours)[3];

RECOMP_IMPORT("mm_recomp_interface_helper", void CGlyphColours_Register(s16 (**pointer)[3][3]));
static s16 (*pCGlyphColours)[3][3];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonPositionsX_Register(s16 (**pointer)[4]));
static s16 (*pButtonPositionsX)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonPositionsY_Register(s16 (**pointer)[4]));
static s16 (*pButtonPositionsY)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonRectSizesX_Register(s16 (**pointer)[4]));
static s16 (*pButtonRectSizesX)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonRectSizesY_Register(s16 (**pointer)[4]));
static s16 (*pButtonRectSizesY)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonScalesX_Register(s16 (**pointer)[4]));
static s16 (*pButtonScalesX)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonScalesY_Register(s16 (**pointer)[4]));
static s16 (*pButtonScalesY)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ItemIconPositionsX_Register(s16 (**pointer)[]));
static s16 (*pItemIconPositionsX)[];

RECOMP_IMPORT("mm_recomp_interface_helper", void ItemIconPositionsY_Register(s16 (**pointer)[]));
static s16 (*pItemIconPositionsY)[];

RECOMP_IMPORT("mm_recomp_interface_helper", void AmmoPositionsTensX_Register(s16 (**pointer)[4]));
static s16 (*pAmmoPositionsTensX)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void AmmoPositionsTensY_Register(s16 (**pointer)[4]));
static s16 (*pAmmoPositionsTensY)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void AmmoPositionsOnesX_Register(s16 (**pointer)[4]));
static s16 (*pAmmoPositionsOnesX)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void AmmoPositionsOnesY_Register(s16 (**pointer)[4]));
static s16 (*pAmmoPositionsOnesY)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void BItemEquipPositionX_Register(s16 (**pointer)));
static s16 (*pBItemEquipPositionX);

RECOMP_IMPORT("mm_recomp_interface_helper", void BItemEquipPositionY_Register(s16 (**pointer)));
static s16 (*pBItemEquipPositionY);

RECOMP_IMPORT("mm_recomp_interface_helper", void AItemEquipPositionX_Register(s16 (**pointer)));
static s16 (*pAItemEquipPositionX);

RECOMP_IMPORT("mm_recomp_interface_helper", void AItemEquipPositionY_Register(s16 (**pointer)));
static s16 (*pAItemEquipPositionY);

RECOMP_IMPORT("mm_recomp_interface_helper", void CUpButtonEnabled_Register(bool (**pointer)));
static bool (*pCUpButtonEnabled);

RECOMP_IMPORT("mm_recomp_interface_helper", void CUpLabelEnabled_Register(bool (**pointer)));
static bool (*pCUpLabelEnabled);

extern f32 D_801BF9CC[];
extern u8 gButtonBackgroundTex[];
extern s16 sMaskCButtonPosX[3];
extern s16 sMaskCButtonPosY[3];
extern s16 sCButtonPosX[3];
extern s16 sCButtonPosY[3];
extern s16 sBButtonDoActionXPositions[2];
extern s16 sBButtonDoActionYPositions[2];

static void set_rgb_s16(s16 color[3], s16 r, s16 g, s16 b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

static void apply_white_button_colours(void) {
    if (pButtonColours != NULL) {
        for (int i = 0; i < 4; i++) {
            set_rgb_s16((*pButtonColours)[i], 255, 255, 255);
        }
    }

    if (pStartButtonColour != NULL) {
        set_rgb_s16(*pStartButtonColour, 255, 255, 255);
    }

    if (pCUpButtonColours != NULL) {
        set_rgb_s16(*pCUpButtonColours, 255, 255, 255);
    }

    if (pCGlyphColours != NULL) {
        for (int i = 0; i < 3; i++) {
            set_rgb_s16((*pCGlyphColours)[i], 255, 255, 255);
        }
    }
}

#define AB_LAYOUT_ACTION_LEFT 0
#define AB_LAYOUT_ATTACK_LEFT 1
#define Z3DS_BUTTON_SOURCE_SIZE 32
#define Z3DS_MAIN_BUTTON_RECT_SIZE 29
#define Z3DS_MAIN_BUTTON_SCALE ((Z3DS_BUTTON_SOURCE_SIZE * 1024) / Z3DS_MAIN_BUTTON_RECT_SIZE)

static u32 get_ab_layout(void) {
    return recomp_get_config_u32("ab_layout");
}

static void apply_3ds_button_layout(void) {
    s16 button_x[4] = { 218, 242, 219, 271 };
    s16 button_y[4] = { 52, 35, 18, 18 };
    s16 b_item_x = 550;
    s16 b_item_y = 690;
    s16 a_item_x = 350;
    s16 a_item_y = 960;

    if (get_ab_layout() == AB_LAYOUT_ATTACK_LEFT) {
        button_x[EQUIP_SLOT_B] = 196;
        button_y[EQUIP_SLOT_B] = 35;
        b_item_x = 365;
        b_item_y = 960;
        a_item_x = 550;
        a_item_y = 690;
    }

    if ((pButtonPositionsX != NULL) && (pButtonPositionsY != NULL)) {
        for (int i = 0; i < 4; i++) {
            (*pButtonPositionsX)[i] = button_x[i];
            (*pButtonPositionsY)[i] = button_y[i];
        }
    }

    if ((pButtonRectSizesX != NULL) && (pButtonRectSizesY != NULL) && (pButtonScalesX != NULL) &&
        (pButtonScalesY != NULL)) {
        for (int i = EQUIP_SLOT_B; i <= EQUIP_SLOT_C_DOWN; i++) {
            (*pButtonRectSizesX)[i] = Z3DS_MAIN_BUTTON_RECT_SIZE;
            (*pButtonRectSizesY)[i] = Z3DS_MAIN_BUTTON_RECT_SIZE;
            (*pButtonScalesX)[i] = Z3DS_MAIN_BUTTON_SCALE;
            (*pButtonScalesY)[i] = Z3DS_MAIN_BUTTON_SCALE;
        }
    }

    if ((pItemIconPositionsX != NULL) && (pItemIconPositionsY != NULL)) {
        for (int i = 0; i < 4; i++) {
            (*pItemIconPositionsX)[i] = button_x[i] << 2;
            (*pItemIconPositionsY)[i] = button_y[i] << 2;
        }
    }

    if ((pAmmoPositionsTensX != NULL) && (pAmmoPositionsTensY != NULL) && (pAmmoPositionsOnesX != NULL) &&
        (pAmmoPositionsOnesY != NULL)) {
        (*pAmmoPositionsTensX)[EQUIP_SLOT_B] = button_x[EQUIP_SLOT_B] + 2;
        (*pAmmoPositionsTensY)[EQUIP_SLOT_B] = button_y[EQUIP_SLOT_B] + 19;

        for (int i = EQUIP_SLOT_C_LEFT; i <= EQUIP_SLOT_C_RIGHT; i++) {
            (*pAmmoPositionsTensX)[i] = button_x[i] + 1;
            (*pAmmoPositionsTensY)[i] = button_y[i] + 17;
        }

        for (int i = EQUIP_SLOT_B; i <= EQUIP_SLOT_C_RIGHT; i++) {
            (*pAmmoPositionsOnesX)[i] = (*pAmmoPositionsTensX)[i] + 6;
            (*pAmmoPositionsOnesY)[i] = (*pAmmoPositionsTensY)[i];
        }
    }

    if ((pBItemEquipPositionX != NULL) && (pBItemEquipPositionY != NULL)) {
        *pBItemEquipPositionX = b_item_x;
        *pBItemEquipPositionY = b_item_y;
    }

    if ((pAItemEquipPositionX != NULL) && (pAItemEquipPositionY != NULL)) {
        *pAItemEquipPositionX = a_item_x;
        *pAItemEquipPositionY = a_item_y;
    }

    sCButtonPosX[0] = sMaskCButtonPosX[0] = 815;
    sCButtonPosY[0] = sMaskCButtonPosY[0] = 950;
    sCButtonPosX[1] = sMaskCButtonPosX[1] = 550;
    sCButtonPosY[1] = sMaskCButtonPosY[1] = 1150;
    sCButtonPosX[2] = sMaskCButtonPosX[2] = 1140;
    sCButtonPosY[2] = sMaskCButtonPosY[2] = 1102;

    sBButtonDoActionXPositions[0] = button_x[EQUIP_SLOT_B] - 9;
    sBButtonDoActionYPositions[0] = button_y[EQUIP_SLOT_B] + 6;
    sBButtonDoActionXPositions[1] = button_x[EQUIP_SLOT_B] - 12;
    sBButtonDoActionYPositions[1] = button_y[EQUIP_SLOT_B] + 5;

    if (pCUpButtonEnabled != NULL) {
        *pCUpButtonEnabled = false;
    }

    if (pCUpLabelEnabled != NULL) {
        *pCUpLabelEnabled = false;
    }
}

static void refresh_hud_style(void) {
    apply_white_button_colours();
    apply_3ds_button_layout();
}

RECOMP_CALLBACK("*", recomp_after_play_init) void after_play_init(void) {
    ButtonColours_Register(&pButtonColours);
    StartButtonColour_Register(&pStartButtonColour);
    CUpButtonColours_Register(&pCUpButtonColours);
    CGlyphColours_Register(&pCGlyphColours);
    ButtonPositionsX_Register(&pButtonPositionsX);
    ButtonPositionsY_Register(&pButtonPositionsY);
    ButtonRectSizesX_Register(&pButtonRectSizesX);
    ButtonRectSizesY_Register(&pButtonRectSizesY);
    ButtonScalesX_Register(&pButtonScalesX);
    ButtonScalesY_Register(&pButtonScalesY);
    ItemIconPositionsX_Register(&pItemIconPositionsX);
    ItemIconPositionsY_Register(&pItemIconPositionsY);
    AmmoPositionsTensX_Register(&pAmmoPositionsTensX);
    AmmoPositionsTensY_Register(&pAmmoPositionsTensY);
    AmmoPositionsOnesX_Register(&pAmmoPositionsOnesX);
    AmmoPositionsOnesY_Register(&pAmmoPositionsOnesY);
    BItemEquipPositionX_Register(&pBItemEquipPositionX);
    BItemEquipPositionY_Register(&pBItemEquipPositionY);
    AItemEquipPositionX_Register(&pAItemEquipPositionX);
    AItemEquipPositionY_Register(&pAItemEquipPositionY);
    CUpButtonEnabled_Register(&pCUpButtonEnabled);
    CUpLabelEnabled_Register(&pCUpLabelEnabled);

    refresh_hud_style();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", button_event_init) void on_button_event_init(PlayState* play, EquipSlot button) {
    refresh_hud_style();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", button_event_return) void on_button_event_return(PlayState* play, EquipSlot button) {
    refresh_hud_style();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", start_button_event_init) void on_start_button_event_init(PlayState* play) {
    apply_white_button_colours();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", start_button_event_return) void on_start_button_event_return(PlayState* play) {
    apply_white_button_colours();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", c_up_button_event_init) void on_c_up_button_event_init(PlayState* play, s16* alpha) {
    apply_white_button_colours();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", c_up_button_event_return) void on_c_up_button_event_return(PlayState* play, s16* alpha) {
    apply_white_button_colours();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", c_glyph_event_init) void on_c_glyph_event_init(PlayState* play) {
    apply_white_button_colours();
}

RECOMP_CALLBACK("mm_recomp_interface_helper", c_glyph_event_return) void on_c_glyph_event_return(PlayState* play) {
    apply_white_button_colours();
}

RECOMP_HOOK("Interface_DrawItemButtons") void on_interface_draw_item_buttons(PlayState* play) {
    refresh_hud_style();
}

RECOMP_HOOK("Interface_DrawCButtonIcons") void on_interface_draw_c_button_icons(PlayState* play) {
    refresh_hud_style();
}

RECOMP_HOOK("Interface_DrawAButton") void on_interface_draw_a_button(PlayState* play) {
    apply_white_button_colours();
}

#define DO_ACTION_TEX_WIDTH 48
#define DO_ACTION_TEX_HEIGHT 16
#define DO_ACTION_TEX_SIZE ((DO_ACTION_TEX_WIDTH * DO_ACTION_TEX_HEIGHT) / 2)

static u8* get_action_label_texture(InterfaceContext* interfaceCtx) {
    s32 action = interfaceCtx->unk_212;

    if (!((interfaceCtx->aButtonState <= A_BTN_STATE_1) || (interfaceCtx->aButtonState == A_BTN_STATE_3))) {
        action = interfaceCtx->aButtonDoAction;
    }

    if ((action < 0) || (action >= DO_ACTION_MAX)) {
        action = DO_ACTION_NONE;
    }

    return sZ3dsActionLabelTextures[action];
}

static void set_action_label_texcoords(InterfaceContext* interfaceCtx) {
    interfaceCtx->actionVtx[8 + 1].v.tc[0] = interfaceCtx->actionVtx[8 + 3].v.tc[0] = Z3DS_ACTION_LABEL_WIDTH << 5;
    interfaceCtx->actionVtx[8 + 2].v.tc[1] = interfaceCtx->actionVtx[8 + 3].v.tc[1] = Z3DS_ACTION_LABEL_HEIGHT << 5;
}

static void get_action_button_view_offset(s32* xOffset, s32* yOffset) {
    if (get_ab_layout() == AB_LAYOUT_ATTACK_LEFT) {
        *xOffset = 18;
        *yOffset = 20;
    } else {
        *xOffset = -4;
        *yOffset = 4;
    }
}

static void set_action_perspective_view(PlayState* play, s32 topY, s32 bottomY, s32 leftX, s32 rightX) {
    s32 xOffset;
    s32 yOffset;

    get_action_button_view_offset(&xOffset, &yOffset);
    Interface_SetPerspectiveView(play, topY + yOffset, bottomY + yOffset, leftX + xOffset, rightX + xOffset);
}

RECOMP_PATCH void Interface_DrawAButton(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 aAlpha;
    u8* actionLabelTex;

    if ((play->pauseCtx.state != 0) || (play->pauseCtx.debugEditor != 0) || (play->pauseCtx.alpha != 0)) {
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    aAlpha = interfaceCtx->aAlpha;

    if (aAlpha > 100) {
        aAlpha = 100;
    }

    Gfx_SetupDL42_Overlay(play->state.gfxCtx);

    set_action_perspective_view(play, 25 + R_A_BTN_Y_OFFSET, 70 + R_A_BTN_Y_OFFSET, 192, 237);

    gSPClearGeometryMode(OVERLAY_DISP++, G_CULL_BOTH);
    gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetAlphaCompare(OVERLAY_DISP++, G_AC_THRESHOLD);

    Matrix_Translate(0.0f, 0.0f, -38.0f, MTXMODE_NEW);
    Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
    Matrix_RotateXFApply(interfaceCtx->aButtonRoll / 10000.0f);

    gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPPipeSync(OVERLAY_DISP++);
    gSPVertex(OVERLAY_DISP++, &interfaceCtx->actionVtx[4], 4, 0);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 0, 0, aAlpha);

    OVERLAY_DISP = Gfx_DrawTexQuadIA8(OVERLAY_DISP, gButtonBackgroundTex, 32, 32, 0);

    gDPPipeSync(OVERLAY_DISP++);
    set_action_perspective_view(play, 23 + R_A_BTN_Y_OFFSET, 68 + R_A_BTN_Y_OFFSET, 190, 235);
    gSPVertex(OVERLAY_DISP++, &interfaceCtx->actionVtx[0], 4, 0);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->aAlpha);
    gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);

    gDPPipeSync(OVERLAY_DISP++);
    set_action_perspective_view(play, 23 + R_A_BTN_Y_OFFSET, 68 + R_A_BTN_Y_OFFSET, 190, 235);
    gSPSetGeometryMode(OVERLAY_DISP++, G_CULL_BACK);
    gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                      ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->aAlpha);
    gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);

    Matrix_Translate(0.0f, 0.0f, D_801BF9CC[gSaveContext.options.language] / 10.0f, MTXMODE_NEW);
    Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
    Matrix_RotateXFApply(interfaceCtx->aButtonRoll / 10000.0f);
    gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    set_action_label_texcoords(interfaceCtx);
    gSPVertex(OVERLAY_DISP++, &interfaceCtx->actionVtx[8], 4, 0);

    actionLabelTex = get_action_label_texture(interfaceCtx);
    if (actionLabelTex != NULL) {
        OVERLAY_DISP = Gfx_DrawTexQuadIA8(OVERLAY_DISP, actionLabelTex, Z3DS_ACTION_LABEL_WIDTH,
                                          Z3DS_ACTION_LABEL_HEIGHT, 0);
    } else if (((interfaceCtx->aButtonState <= A_BTN_STATE_1) || (interfaceCtx->aButtonState == A_BTN_STATE_3))) {
        OVERLAY_DISP = Gfx_DrawTexQuad4b(OVERLAY_DISP, interfaceCtx->doActionSegment, 3, DO_ACTION_TEX_WIDTH,
                                         DO_ACTION_TEX_HEIGHT, 0);
    } else {
        OVERLAY_DISP = Gfx_DrawTexQuad4b(OVERLAY_DISP, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE, 3,
                                         DO_ACTION_TEX_WIDTH, DO_ACTION_TEX_HEIGHT, 0);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
