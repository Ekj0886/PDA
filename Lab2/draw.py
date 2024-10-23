import sys
import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def draw_block(ax, x, y, width, height, block_id, color):
    """Draw a colored rectangle block and mark its ID."""
    ax.add_patch(
        patches.Rectangle(
            (x, y),
            width,
            height,
            fill=True,
            edgecolor="#000000",  # Black border for the block
            facecolor=color,      # Color for the block (red if overlapping)
            alpha=1.0,
            clip_on=False         # Allow block to extend beyond the axis boundary
        )
    )
    
    # Add the ID text in the center of the block
    text_x = x + width / 2  # Center x
    text_y = y + height / 2  # Center y
    ax.text(text_x, text_y, str(block_id), fontsize=15, ha='center', va='center', color='white', clip_on=False)

def check_overlap(x, y, width, height, window_width, window_height):
    """Check if the block overlaps with the outline of the window."""
    if x < 0 or y < 0 or (x + width) > window_width or (y + height) > window_height:
        return True
    return False

def main():
    # Read command-line arguments for input and output file names
    txt_name = sys.argv[1] 
    png_name = sys.argv[2] 

    # Create an 'image' directory if it does not exist
    output_dir = 'FloorPlan_image'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Read input from the text file
    with open(txt_name, 'r') as fread:
        f = fread.read().strip().split("\n")

    total_block_number = int(f[0])  # Number of blocks
    window_width, window_height = map(int, f[1].split(" "))  # Window dimensions

    # Calculate bounds for the entire area (including blocks outside the outline)
    min_x, min_y = 0, 0
    max_x, max_y = window_width, window_height

    # Find the minimum and maximum x, y coordinates including blocks that extend outside
    blocks = []
    for i in range(2, total_block_number + 2):  # Start from line 2 to total blocks
        ss = f[i].split(" ")
        block_id = ss[0]  # ID of the block
        x = int(ss[1])  # Lower-left x coordinate
        y = int(ss[2])  # Lower-left y coordinate
        width = int(ss[3])  # Width of the block
        height = int(ss[4])  # Height of the block
        
        # Track the bounds to adjust image size
        blocks.append((x, y, width, height, block_id))
        min_x = min(min_x, x)
        min_y = min(min_y, y)
        max_x = max(max_x, x + width)
        max_y = max(max_y, y + height)

    # Create a figure, adjust size dynamically to cover all blocks
    fig = plt.figure(figsize=(16, 12))
    ax = fig.add_subplot(111)
    

    # Adjust the axis limits based on min/max values of blocks and outline
    Wl = 0.05*(max_x - min_x)
    Hl = 0.05*(max_y - min_y)
    ax.set_xlim(min_x - Wl, max_x + Wl)  # Add 10 pixels margin
    ax.set_ylim(min_y - Hl, max_y + Hl)

    # Draw the outline of the window (even if some blocks extend outside)

    # Draw each block, adjusting the color if it overlaps with the window outline
    for block in blocks:
        x, y, width, height, block_id = block
        is_overlap = check_overlap(x, y, width, height, window_width, window_height)
        block_color = "red" if is_overlap else "#3361C4"
        draw_block(ax, x, y, width, height, block_id, block_color)

    ax.add_patch(
        patches.Rectangle(
            (0, 0),
            window_width,
            window_height,
            fill=False,  # No fill, just the outline
            edgecolor="#fff13f",  # Black border for the window
            linewidth=4
        )
    )

    # Save the figure as a PNG file in the 'dump' directory
    output_file_path = os.path.join(output_dir, png_name)
    plt.savefig(output_file_path, bbox_inches='tight')
    plt.close(fig)  # Close the figure to free up memory
    print(f"Image saved as {output_file_path}")

if __name__ == "__main__":
    main()
