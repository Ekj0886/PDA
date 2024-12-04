import matplotlib.pyplot as plt
import random
import sys

def draw_route(file_path, output_image, dpi=300, margin_ratio=0.05):
    with open(file_path, 'r') as f:
        lines = f.readlines()

    # Parse the first three lines
    route_area = list(map(int, lines[0].strip().split()))
    chip1_area = list(map(int, lines[1].strip().split()))
    chip2_area = list(map(int, lines[2].strip().split()))

    # Parse the remaining lines as nets
    nets = []
    for line in lines[3:]:
        nets.append(list(map(int, line.strip().split())))

    # Calculate margin based on the routing area size
    margin_x = route_area[2] * margin_ratio
    margin_y = route_area[3] * margin_ratio

    # Create the plot
    fig, ax = plt.subplots(figsize=(10, 8))

    # Set axis limits to include the routing area and margins
    ax.set_xlim(route_area[0] - margin_x, route_area[0] + route_area[2] + margin_x)
    ax.set_ylim(route_area[1] - margin_y, route_area[1] + route_area[3] + margin_y)

     # Draw the routing area with a black outline
    ax.add_patch(plt.Rectangle(
        (route_area[0], route_area[1]),  # Bottom-left corner
        route_area[2],  # Width
        route_area[3],  # Height
        edgecolor='black',  # Black border
        facecolor='none',  # No fill
        lw=1.5  # Line width
    ))

    # Draw chip1 and chip2 areas with a light blue color (half-transparent)
    chip1_color = (0.5, 0.5, 1, 0.5)  # Light blue with transparency
    chip2_color = (0.5, 0.5, 1, 0.5)  # Light blue with transparency
    ax.add_patch(plt.Rectangle((chip1_area[0], chip1_area[1]), chip1_area[2], chip1_area[3],
                               edgecolor='black', facecolor=chip1_color, lw=0.5))
    ax.add_patch(plt.Rectangle((chip2_area[0], chip2_area[1]), chip2_area[2], chip2_area[3],
                               edgecolor='black', facecolor=chip2_color, lw=0.5))

    # Plot each net with random colors
    for net in nets:
        net_coords = net[0:]  # All coordinates of the net
        src_x, src_y = net_coords[0], net_coords[1]
        snk_x, snk_y = net_coords[-2], net_coords[-1]

        # Generate a random color for this net
        color = (random.random(), random.random(), random.random())  # RGB

        # Plot the points for this net (source, intermediate points, and sink)
        for i in range(0, len(net_coords) - 2, 2):
            x1, y1 = net_coords[i], net_coords[i + 1]
            x2, y2 = net_coords[i + 2], net_coords[i + 3]

            # Plot line between consecutive points
            ax.plot([x1, x2], [y1, y2], color=color, lw=1)

        # Plot the source and sink points
        ax.scatter(src_x, src_y, color=color, s=10, zorder=5)  # Source point
        ax.scatter(snk_x, snk_y, color=color, s=10, zorder=5)  # Sink point

    # Remove axis labels and ticks
    ax.set_xticks([])
    ax.set_yticks([])

    # Fix the aspect ratio to ensure equal scaling for x and y
    ax.set_aspect('equal', adjustable='box')

    # Save the image as PNG with specified DPI for high quality
    plt.savefig(output_image, format='png', dpi=dpi, bbox_inches='tight', pad_inches=0)
    plt.close()

# Command line entry point
if __name__ == "__main__":
    # Ensure correct number of arguments
    if len(sys.argv) != 3:
        print("Usage: python draw.py <route_file> <output_image>")
        sys.exit(1)

    # Get the input file and output image file from command line arguments
    route_file = sys.argv[1]
    output_image = sys.argv[2]

    # Draw the route and save the image
    draw_route(route_file, output_image)
