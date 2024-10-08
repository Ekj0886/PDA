import sys
import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def draw_block(ax, x, y, width, height, block_id):
    """Draw a colored rectangle block and mark its ID."""
    color = "#3361C4"  # Default color
    # Draw the rectangle
    ax.add_patch(
        patches.Rectangle( 
            (x, y),
            width,
            height,
            fill=True,
            edgecolor="#000000",
            facecolor=color,
            alpha=1.0
        )
    )
    
    # Add the ID text in the center of the block
    text_x = x + width / 2  # Center x
    text_y = y + height / 2  # Center y
    ax.text(text_x, text_y, str(block_id), fontsize=10, ha='center', va='center', color='white')

def main():
    # Read command-line arguments for input and output file names
    txt_name = sys.argv[1] 
    png_name = sys.argv[2] 

    # Create an 'image' directory if it does not exist
    output_dir = 'draw'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Read input from the text file
    with open(txt_name, 'r') as fread:
        f = fread.read().strip().split("\n")

    total_block_number = int(f[0])  # Number of blocks
    window_width, window_height = map(int, f[1].split(" "))  # Window dimensions

    # Create a figure
    fig = plt.figure(figsize=(16, 12))  # Size in inches for output

    ax = fig.add_subplot(111)
    ax.set_xlim(0, window_width)
    ax.set_ylim(0, window_height)

    # Read and draw each block
    for i in range(2, total_block_number + 2):  # Start from line 2 to total blocks
        ss = f[i].split(" ")
        block_id = int(ss[0])  # ID of the block
        x = int(ss[1])  # Lower-left x coordinate
        y = int(ss[2])  # Lower-left y coordinate
        width = int(ss[3])  # Width of the block
        height = int(ss[4])  # Height of the block
        draw_block(ax, x, y, width, height, block_id)

    # Save the figure as a PNG file in the 'image' directory
    output_file_path = os.path.join(output_dir, png_name)
    plt.savefig(output_file_path, bbox_inches='tight')
    plt.close(fig)  # Close the figure to free up memory
    print(f"Image saved as {output_file_path}")

if __name__ == "__main__":
    main()
