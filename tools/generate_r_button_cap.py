"""Generate the HUD's TPHD-inspired R cap from its existing round button art."""

from pathlib import Path

try:
    from PIL import Image, ImageDraw
except ImportError as exc:
    raise SystemExit("Pillow is required: python3 -m pip install -r requirements-dev.txt") from exc


ROOT = Path(__file__).resolve().parent.parent
SOURCE = ROOT / "package/textures/o2r/alt/parameter_static/gButtonBackgroundTex.png"
OUTPUT_PNG = ROOT / "assets/r_button_cap.png"
OUTPUT_IA8 = ROOT / "assets/r_button_cap.ia8.bin"
SCALE = 4
WIDTH = 64
HEIGHT = 32


def quadratic(start, control, end, steps=8):
    """Return points along a quadratic curve, excluding its first point."""
    points = []
    for step in range(1, steps + 1):
        t = step / steps
        one_minus_t = 1.0 - t
        x = one_minus_t * one_minus_t * start[0] + 2 * one_minus_t * t * control[0] + t * t * end[0]
        y = one_minus_t * one_minus_t * start[1] + 2 * one_minus_t * t * control[1] + t * t * end[1]
        points.append((x, y))
    return points


def cap_outline(inset=0):
    left = 2 + inset
    right = 62 - inset
    bottom = 29 - inset

    # A shallow asymmetric arc gives the cap its shoulder-button profile.
    points = [(left, 13 + inset)]
    points += quadratic(points[-1], (28, -4 + inset), (right, 16 + inset), steps=24)
    points.append((right, 24 - inset))
    points += quadratic(points[-1], (right, bottom), (57 - inset, bottom))
    points.append((6 + inset, bottom))
    points += quadratic(points[-1], (left, bottom), (left, 25 - inset))
    points.append((left, 13 + inset))
    return points


def make_cap() -> Image.Image:
    circle = Image.open(SOURCE).convert("RGBA")
    canvas = Image.new("RGBA", (WIDTH * SCALE, HEIGHT * SCALE), (0, 0, 0, 0))

    mask = Image.new("L", canvas.size, 0)
    draw_mask = ImageDraw.Draw(mask)
    points = cap_outline()
    draw_mask.polygon([(x * SCALE, y * SCALE) for x, y in points], fill=110)

    # Match the face button's translucent fill and brighter rim.
    inner_points = cap_outline(inset=2)
    draw_mask.polygon([(x * SCALE, y * SCALE) for x, y in inner_points], fill=46)

    # Preserve the source art's intensity so the normal HUD colour pipeline
    # treats the cap like the other buttons.
    center_colour = circle.getpixel((circle.width // 2, circle.height // 2))[:3]
    canvas.paste((*center_colour, 255), (0, 0, canvas.width, canvas.height))
    canvas.putalpha(mask)

    return canvas.resize((WIDTH, HEIGHT), Image.Resampling.LANCZOS)


def encode_ia8(image: Image.Image) -> bytes:
    encoded = bytearray()
    for red, green, blue, alpha in image.convert("RGBA").get_flattened_data():
        intensity = round((red * 0.299 + green * 0.587 + blue * 0.114) / 17)
        encoded.append((max(0, min(15, intensity)) << 4) | round(alpha / 17))
    return bytes(encoded)


def main() -> None:
    cap = make_cap()
    OUTPUT_PNG.parent.mkdir(parents=True, exist_ok=True)
    cap.save(OUTPUT_PNG)
    OUTPUT_IA8.write_bytes(encode_ia8(cap))


if __name__ == "__main__":
    main()
