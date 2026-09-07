#include "modding.h"
#include "global.h"
#include "action_label_textures.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define INCBIN(identifier, filename)          \
    asm(".pushsection .rodata\n"              \
        "\t.globl " #identifier "\n"         \
        "\t.type " #identifier ", @object\n" \
        "\t.balign 8\n"                      \
        #identifier ":\n"                     \
        "\t.incbin \"" filename "\"\n\n"      \
        "\t.balign 8\n"                      \
        "\t.popsection\n");                  \
    extern u8 identifier[]

INCBIN(sRButtonCapTexture, "assets/r_button_cap.ia8.bin");

void Interface_SetPerspectiveView(PlayState* play, s32 topY, s32 bottomY, s32 leftX, s32 rightX);
RECOMP_IMPORT("*", u32 recomp_get_config_u32(const char* key));

RECOMP_IMPORT("mm_recomp_interface_helper", void AmmoFullColour_Register(int (**pointer)[3]));
static int (*pAmmoFullColour)[3];

RECOMP_IMPORT("mm_recomp_interface_helper", void AmmoEmptyColour_Register(int (**pointer)[3]));
static int (*pAmmoEmptyColour)[3];
static s16 sAmmoDrawAlpha = 255;

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonColours_Register(s16 (**pointer)[4][3]));
static s16 (*pButtonColours)[4][3];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonTextures_Register(TexturePtr (**pointer)[4]));
static TexturePtr (*pButtonTextures)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonTexturesWidth_Register(s16 (**pointer)[4]));
static s16 (*pButtonTexturesWidth)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void ButtonTexturesHeight_Register(s16 (**pointer)[4]));
static s16 (*pButtonTexturesHeight)[4];

RECOMP_IMPORT("mm_recomp_interface_helper", void StartButtonColour_Register(s16 (**pointer)[3]));
static s16 (*pStartButtonColour)[3];

RECOMP_IMPORT("mm_recomp_interface_helper", void CUpButtonColours_Register(s16 (**pointer)[3]));
static s16 (*pCUpButtonColours)[3];

RECOMP_IMPORT("mm_recomp_interface_helper", void CGlyphColours_Register(s16 (**pointer)[3][3]));
static s16 (*pCGlyphColours)[3][3];

RECOMP_IMPORT("mm_recomp_interface_helper", void CGlyphsEnabled_Register(bool (**pointer)[3]));
static bool (*pCGlyphsEnabled)[3];

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

RECOMP_IMPORT("mm_recomp_interface_helper", void ItemIconRectSizesX_Register(s16 (**pointer)[]));
static s16 (*pItemIconRectSizesX)[];

RECOMP_IMPORT("mm_recomp_interface_helper", void ItemIconRectSizesY_Register(s16 (**pointer)[]));
static s16 (*pItemIconRectSizesY)[];

RECOMP_IMPORT("mm_recomp_interface_helper", void ItemIconTextureScales_Register(s16 (**pointer)[]));
static s16 (*pItemIconTextureScales)[];
static GraphicsContext* sAttackItemGfxCtx = NULL;
static Gfx* sAttackItemDisplayListStart = NULL;

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

static void set_rgb_int(int color[3], int r, int g, int b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

static void apply_ammo_colours(void) {
    if (pAmmoFullColour != NULL) {
        set_rgb_int(*pAmmoFullColour, 255, 210, 64);
    }

    if (pAmmoEmptyColour != NULL) {
        set_rgb_int(*pAmmoEmptyColour, 255, 255, 255);
    }
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

typedef enum {
    AB_LAYOUT_ACTION_LEFT,
    AB_LAYOUT_ATTACK_LEFT,
    AB_LAYOUT_NINTENDO,
} AbLayout;

#define Z3DS_BUTTON_SOURCE_SIZE 32
#define Z3DS_MAIN_BUTTON_RECT_SIZE 29
#define Z3DS_MAIN_BUTTON_SCALE ((Z3DS_BUTTON_SOURCE_SIZE * 1024) / Z3DS_MAIN_BUTTON_RECT_SIZE)

// Equal radii keep opposite sides of the face-button diamond parallel.
#define FACE_BUTTON_CENTER_X 219
#define FACE_BUTTON_CENTER_Y 35
#define FACE_BUTTON_RADIUS_X 19
#define FACE_BUTTON_RADIUS_Y 19
#define SHOULDER_ITEM_X 259
#define SHOULDER_ITEM_Y 19
#define R_BUTTON_SOURCE_WIDTH 64
#define R_BUTTON_SOURCE_HEIGHT 32
#define R_BUTTON_RECT_WIDTH 22
#define R_BUTTON_RECT_HEIGHT 12
#define R_BUTTON_SCALE_X ((R_BUTTON_SOURCE_WIDTH * 1024) / R_BUTTON_RECT_WIDTH)
#define R_BUTTON_SCALE_Y ((R_BUTTON_SOURCE_HEIGHT * 1024) / R_BUTTON_RECT_HEIGHT)
#define ITEM_ICON_SOURCE_SIZE 32
#define ITEM_ICON_RECT_SIZE 20
#define ITEM_ICON_RECT_SIZE_QUARTERS (ITEM_ICON_RECT_SIZE << 2)
#define ITEM_ICON_SCALE ((ITEM_ICON_SOURCE_SIZE * 1024) / ITEM_ICON_RECT_SIZE)
#define ATTACK_ITEM_ICON_RECT_SIZE 18
#define ATTACK_ITEM_ICON_RECT_SIZE_QUARTERS (ATTACK_ITEM_ICON_RECT_SIZE << 2)
#define ATTACK_ITEM_ICON_SCALE ((ITEM_ICON_SOURCE_SIZE * 1024) / ATTACK_ITEM_ICON_RECT_SIZE)
#define ATTACK_ITEM_ICON_OFFSET_X 2
#define ATTACK_ITEM_ICON_OFFSET_Y 0

static AbLayout get_ab_layout(void) {
    return (AbLayout)recomp_get_config_u32("ab_layout");
}

static void apply_3ds_button_layout(void) {
    AbLayout ab_layout = get_ab_layout();
    s16 button_x[4] = {
        FACE_BUTTON_CENTER_X,
        FACE_BUTTON_CENTER_X + FACE_BUTTON_RADIUS_X,
        FACE_BUTTON_CENTER_X,
        SHOULDER_ITEM_X,
    };
    s16 button_y[4] = {
        FACE_BUTTON_CENTER_Y + FACE_BUTTON_RADIUS_Y,
        FACE_BUTTON_CENTER_Y,
        FACE_BUTTON_CENTER_Y - FACE_BUTTON_RADIUS_Y,
        SHOULDER_ITEM_Y,
    };
    s16 b_item_x = 550;
    s16 b_item_y = 690;
    s16 a_item_x = 350;
    s16 a_item_y = 960;

    if (ab_layout == AB_LAYOUT_ATTACK_LEFT) {
        button_x[EQUIP_SLOT_B] = FACE_BUTTON_CENTER_X - FACE_BUTTON_RADIUS_X;
        button_y[EQUIP_SLOT_B] = FACE_BUTTON_CENTER_Y;
        b_item_x = 365;
        b_item_y = 960;
        a_item_x = 550;
        a_item_y = 690;
    } else if (ab_layout == AB_LAYOUT_NINTENDO) {
        // Nintendo face-button order: B/attack south and A/action east.
        // Move the displaced C-Left item to the west point of the diamond.
        button_x[EQUIP_SLOT_C_LEFT] = FACE_BUTTON_CENTER_X - FACE_BUTTON_RADIUS_X;
        button_y[EQUIP_SLOT_C_LEFT] = FACE_BUTTON_CENTER_Y;
        a_item_x = 750;
        a_item_y = 960;
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

        // This cap is derived from the same circular art as the face buttons,
        // then tapered to match TPHD's shoulder-button silhouette.
        (*pButtonRectSizesX)[EQUIP_SLOT_C_RIGHT] = R_BUTTON_RECT_WIDTH;
        (*pButtonRectSizesY)[EQUIP_SLOT_C_RIGHT] = R_BUTTON_RECT_HEIGHT;
        (*pButtonScalesX)[EQUIP_SLOT_C_RIGHT] = R_BUTTON_SCALE_X;
        (*pButtonScalesY)[EQUIP_SLOT_C_RIGHT] = R_BUTTON_SCALE_Y;
    }

    if ((pButtonTextures != NULL) && (pButtonTexturesWidth != NULL) && (pButtonTexturesHeight != NULL)) {
        (*pButtonTextures)[EQUIP_SLOT_C_RIGHT] = sRButtonCapTexture;
        (*pButtonTexturesWidth)[EQUIP_SLOT_C_RIGHT] = R_BUTTON_SOURCE_WIDTH;
        (*pButtonTexturesHeight)[EQUIP_SLOT_C_RIGHT] = R_BUTTON_SOURCE_HEIGHT;
    }

    if (pCGlyphsEnabled != NULL) {
        // This is an item-slot silhouette, not a labeled face button. Suppress
        // the old C-Right glyph when the slot is empty.
        (*pCGlyphsEnabled)[EQUIP_SLOT_C_RIGHT - EQUIP_SLOT_C_LEFT] = false;
    }

    if ((pItemIconPositionsX != NULL) && (pItemIconPositionsY != NULL)) {
        for (int i = EQUIP_SLOT_B; i <= EQUIP_SLOT_C_DOWN; i++) {
            s16 item_rect_size = (i == EQUIP_SLOT_B) ? ATTACK_ITEM_ICON_RECT_SIZE : ITEM_ICON_RECT_SIZE;

            // Positions use quarter-pixels. Centre the smaller icon exactly
            // within each 29x29 circular face-button rectangle.
            (*pItemIconPositionsX)[i] = (button_x[i] << 2) +
                                       ((Z3DS_MAIN_BUTTON_RECT_SIZE - item_rect_size) << 1);
            (*pItemIconPositionsY)[i] = (button_y[i] << 2) +
                                       ((Z3DS_MAIN_BUTTON_RECT_SIZE - item_rect_size) << 1);
            if (i == EQUIP_SLOT_B) {
                (*pItemIconPositionsX)[i] += ATTACK_ITEM_ICON_OFFSET_X * 4;
                (*pItemIconPositionsY)[i] += ATTACK_ITEM_ICON_OFFSET_Y * 4;
            }
        }
        // Centre the smaller item over the 22x12 shoulder cap.
        (*pItemIconPositionsX)[EQUIP_SLOT_C_RIGHT] = (button_x[EQUIP_SLOT_C_RIGHT] << 2) +
                                                     ((R_BUTTON_RECT_WIDTH - ITEM_ICON_RECT_SIZE) * 2);
        (*pItemIconPositionsY)[EQUIP_SLOT_C_RIGHT] = (button_y[EQUIP_SLOT_C_RIGHT] << 2) +
                                                     ((R_BUTTON_RECT_HEIGHT - ITEM_ICON_RECT_SIZE) * 2);
    }

    if ((pItemIconRectSizesX != NULL) && (pItemIconRectSizesY != NULL) && (pItemIconTextureScales != NULL)) {
        for (int i = 0; i < 4; i++) {
            bool is_attack_item = (i == EQUIP_SLOT_B);
            (*pItemIconRectSizesX)[i] = is_attack_item ? ATTACK_ITEM_ICON_RECT_SIZE_QUARTERS
                                                       : ITEM_ICON_RECT_SIZE_QUARTERS;
            (*pItemIconRectSizesY)[i] = is_attack_item ? ATTACK_ITEM_ICON_RECT_SIZE_QUARTERS
                                                       : ITEM_ICON_RECT_SIZE_QUARTERS;
            (*pItemIconTextureScales)[i] = is_attack_item ? ATTACK_ITEM_ICON_SCALE : ITEM_ICON_SCALE;
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
    apply_ammo_colours();
    apply_white_button_colours();
    apply_3ds_button_layout();
}

RECOMP_CALLBACK("*", recomp_after_play_init) void after_play_init(void) {
    AmmoFullColour_Register(&pAmmoFullColour);
    AmmoEmptyColour_Register(&pAmmoEmptyColour);
    ButtonColours_Register(&pButtonColours);
    ButtonTextures_Register(&pButtonTextures);
    ButtonTexturesWidth_Register(&pButtonTexturesWidth);
    ButtonTexturesHeight_Register(&pButtonTexturesHeight);
    StartButtonColour_Register(&pStartButtonColour);
    CUpButtonColours_Register(&pCUpButtonColours);
    CGlyphColours_Register(&pCGlyphColours);
    CGlyphsEnabled_Register(&pCGlyphsEnabled);
    ButtonPositionsX_Register(&pButtonPositionsX);
    ButtonPositionsY_Register(&pButtonPositionsY);
    ButtonRectSizesX_Register(&pButtonRectSizesX);
    ButtonRectSizesY_Register(&pButtonRectSizesY);
    ButtonScalesX_Register(&pButtonScalesX);
    ButtonScalesY_Register(&pButtonScalesY);
    ItemIconPositionsX_Register(&pItemIconPositionsX);
    ItemIconPositionsY_Register(&pItemIconPositionsY);
    ItemIconRectSizesX_Register(&pItemIconRectSizesX);
    ItemIconRectSizesY_Register(&pItemIconRectSizesY);
    ItemIconTextureScales_Register(&pItemIconTextureScales);
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

RECOMP_HOOK("Interface_DrawAmmoCount") void on_interface_draw_ammo_count(PlayState* play, s16 button, s16 alpha) {
    sAmmoDrawAlpha = alpha;
}

RECOMP_CALLBACK("mm_recomp_interface_helper", ammo_event_init) void on_ammo_event_init(PlayState* play, u8 item,
                                                                                      u16* ammo) {
    OPEN_DISPS(play->state.gfxCtx);

    // Interface Helper exposes full and empty colours; its event supplies the
    // missing normal state.
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, sAmmoDrawAlpha);

    CLOSE_DISPS(play->state.gfxCtx);
    apply_ammo_colours();
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

RECOMP_HOOK("Interface_DrawItemIconTexture") void on_interface_draw_item_icon_texture(PlayState* play,
                                                                                       TexturePtr texture,
                                                                                       s16 button) {
    if (button == EQUIP_SLOT_B) {
        sAttackItemGfxCtx = play->state.gfxCtx;
        sAttackItemDisplayListStart = play->state.gfxCtx->overlay.p;
    } else {
        sAttackItemGfxCtx = NULL;
        sAttackItemDisplayListStart = NULL;
    }
}

RECOMP_HOOK_RETURN("Interface_DrawItemIconTexture") void on_interface_draw_item_icon_texture_return(void) {
    if ((sAttackItemGfxCtx != NULL) && (sAttackItemDisplayListStart != NULL) && (pItemIconPositionsX != NULL) &&
        (pItemIconPositionsY != NULL)) {
        s16 left = (*pItemIconPositionsX)[EQUIP_SLOT_B];
        s16 top = (*pItemIconPositionsY)[EQUIP_SLOT_B];
        Gfx* display_list_end = sAttackItemGfxCtx->overlay.p;

        // The helper has no per-mod attack-icon scale. Replace the rectangle it
        // emitted while leaving its code and the high-resolution texture alone.
        for (Gfx* command = sAttackItemDisplayListStart; command + 2 < display_list_end; command++) {
            if ((command->words.w0 >> 24) == G_TEXRECT) {
                Gfx* item_rect = command;
                gSPTextureRectangle(item_rect++, left, top, left + ATTACK_ITEM_ICON_RECT_SIZE_QUARTERS,
                                    top + ATTACK_ITEM_ICON_RECT_SIZE_QUARTERS, G_TX_RENDERTILE, 0, 0,
                                    ATTACK_ITEM_ICON_SCALE, ATTACK_ITEM_ICON_SCALE);
                break;
            }
        }
    }

    sAttackItemGfxCtx = NULL;
    sAttackItemDisplayListStart = NULL;
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
    AbLayout ab_layout = get_ab_layout();

    if (ab_layout == AB_LAYOUT_ATTACK_LEFT) {
        // Compensate for the action renderer's slightly right-shifted
        // perspective window so the South prompt shares the diamond axis.
        *xOffset = 21;
        *yOffset = 24;
    } else if (ab_layout == AB_LAYOUT_NINTENDO) {
        // Mirror the west action-button viewport across the diamond centre.
        *xOffset = 40;
        *yOffset = 4;
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
