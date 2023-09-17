"""Merge normal and topo images into a single image with 4 channels.

The normal image is the RGB channels, and the topo image is the alpha channel.
"""

from pathlib import Path
import cv2


def combine_img_channels(normal_path: Path, topo_path: Path):
    normal_img = cv2.imread(str(normal_path))
    topo_img = cv2.imread(str(topo_path), cv2.IMREAD_GRAYSCALE)
    assert (
        topo_img.shape[:2] == normal_img.shape[:2]
    ), "Images must have the same dimensions."
    combined_img = cv2.merge((normal_img, topo_img))
    output_path = normal_path.with_name(f"{normal_path.stem}-topo.jpg")
    cv2.imwrite(str(output_path), combined_img)
    print(f"Combined image saved to {output_path}")


if __name__ == "__main__":
    for topo_img in Path(__file__).parent.glob("*_*k_topo.jpg"):
        normal_img = topo_img.with_name(
            topo_img.stem.replace("topo", "normal") + ".jpg"
        )
        assert normal_img.exists(), f"Normal image {normal_img} does not exist."
        output_path = topo_img.with_name(f"{topo_img.stem}_combined.jpg")
        combine_img_channels(normal_img, topo_img)
    for normal_img in Path(__file__).parent.glob("*_*k_normal.jpg"):
        assert normal_img.with_name(
            normal_img.stem.replace("normal", "topo") + ".jpg"
        ).exists(), f"Topo image {normal_img} does not exist."
