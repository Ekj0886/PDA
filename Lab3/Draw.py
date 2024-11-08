import sys
import os
import matplotlib.patches as patches
import matplotlib.pyplot as plt
from matplotlib.collections import PatchCollection

# Function to read and parse the placement data
def parse_placement_data(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
    
    # First line: Die boundaries
    die_bounds = list(map(float, lines[0].split()))
    die_lower_x, die_lower_y, die_upper_x, die_upper_y = die_bounds

    # Subsequent lines: Cell data
    cells = []
    for line in lines[1:]:
        cell_data = line.split()
        lowerleft_x = float(cell_data[0])
        lowerleft_y = float(cell_data[1])
        width = float(cell_data[2])
        height = float(cell_data[3])
        fix = int(cell_data[4])
        cells.append((lowerleft_x, lowerleft_y, width, height, fix))
    
    return die_lower_x, die_lower_y, die_upper_x, die_upper_y, cells

# Function to plot placement and save as a high-resolution PNG
def plot_placement(die_lower_x, die_lower_y, die_upper_x, die_upper_y, cells, png_name):
    # Increase figure size for large placements
    fig, ax = plt.subplots(figsize=(20, 20))  # Adjust figsize as needed
    
    # Draw the die area with a thinner border line
    die_width = die_upper_x - die_lower_x
    die_height = die_upper_y - die_lower_y
    die_rect = patches.Rectangle((die_lower_x, die_lower_y), die_width, die_height,
                                 linewidth=0.5, edgecolor='black', facecolor='none')  # Thinner border line
    ax.add_patch(die_rect)
    
    # Draw each cell with a border color based on "fix" status and white face color
    cell_patches = []
    edge_colors = []
    for (lowerleft_x, lowerleft_y, width, height, fix) in cells:
        edge_color = 'red' if fix == 1 else 'blue'
        cell_rect = patches.Rectangle((lowerleft_x, lowerleft_y), width, height, facecolor='white')
        cell_patches.append(cell_rect)
        edge_colors.append(edge_color)
    
    # Add all cells to the plot as a PatchCollection with custom edge colors
    cell_collection = PatchCollection(cell_patches, facecolor='white', edgecolor=edge_colors, linewidths=0.5, alpha=1)
    ax.add_collection(cell_collection)
    
    # Set the plot limits and aspect ratio
    ax.set_xlim(die_lower_x, die_upper_x)
    ax.set_ylim(die_lower_y, die_upper_y)
    ax.set_aspect('equal', adjustable='box')
    ax.set_title('Die Placement Visualization')
    plt.xlabel("X Coordinate")
    plt.ylabel("Y Coordinate")
    plt.grid(False)  # Remove grid for a cleaner look

    # Save the plot as a high-resolution PNG file
    plt.savefig(png_name, format='png', dpi=600)  # Increase DPI for higher quality
    plt.close(fig)  # Close the figure to free memory

# Main function to execute parsing and plotting
if __name__ == "__main__":
    txt_name = sys.argv[1] 
    png_name = sys.argv[2] 

    # Parse data and plot
    die_lower_x, die_lower_y, die_upper_x, die_upper_y, cells = parse_placement_data(txt_name)
    plot_placement(die_lower_x, die_lower_y, die_upper_x, die_upper_y, cells, png_name)
