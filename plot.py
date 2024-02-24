import matplotlib.pyplot as plt
from matplotlib import collections as mc

file_path = "result.txt"

def line_intersection(edge, x=None, y=None):
    """Calculate the intersection point of a line and a vertical or horizontal line."""
    (x1, y1), (x2, y2) = edge
    if x is not None:
        y = y1 + (x - x1) * (y2 - y1) / (x2 - x1)
    elif y is not None:
        x = x1 + (y - y1) * (x2 - x1) / (y2 - y1)
    return (x, y)

def truncate(edges, min_x, max_x, min_y, max_y):
    truncated_edges = []
    for edge in edges:
        start, end = edge
        if start[0] < min_x:
            start = line_intersection((start,end), x=min_x)
        elif start[0] > max_x:
            start = line_intersection((start,end), x=max_x)
        if start[1] < min_y:
            start = line_intersection((start,end), y=min_y)
        elif start[1] > max_y:
            start = line_intersection((start,end), y=max_y)
        if end[0] < min_x:
            end = line_intersection((start,end), x=min_x)
        elif end[0] > max_x:
            end = line_intersection((start,end), x=max_x)
        if end[1] < min_y:
            end = line_intersection((start,end), y=min_y)
        elif end[1] > max_y:
            end = line_intersection((start,end), y=max_y)
        truncated_edges.append((start, end))
    return truncated_edges

with open(file_path, "r") as file:
    l1 = file.readlines()
    num_edges, num_vertices = [int(i) for i in l1[-1].strip().split()]
    print(num_edges, num_vertices)
    edges = l1[-num_edges-1:-1]
    lines = []
    print(edges)
    for j in edges:
        x1, y1, x2, y2 = [float(k) for k in j.strip().split()]
        lines.append([(x1, y1), (x2, y2)])
    points = l1[-1-num_edges-num_vertices:-1-num_edges]
    print(points)
    fig, ax = plt.subplots()  # Create the figure and axis here
    

    x = []
    y = []
    for j in points:
        x1, y1 = [float(k) for k in j.strip().split()]
        #print(x1, y1)
        x.append(x1)
        y.append(y1)
    
    min_x = min(x1 for x1 in x)
    max_x = max(x1 for x1 in x)
    min_y = min(y1 for y1 in y)
    max_y = max(y1 for y1 in y)

    min_x -= 0.2*(max_x - min_x)
    max_x += 0.2*(max_x - min_x)
    min_y -= 0.2*(max_y - min_y)
    max_y += 0.2*(max_y - min_y)

    truncated_edges = truncate(lines, min_x, max_x, min_y, max_y)
    lc = mc.LineCollection(truncated_edges, linewidths=2, colors = 'black')  # Create a collection of lines
    #lc = mc.LineCollection(lines, linewidths=2)  # Create a collection of lines
    ax.add_collection(lc)  # Add lines to the same axis
    ax.scatter(x, y, color='red')  # Add points to the same axis
    #ax.axis('equal')  # Set the aspect ratio of the plot to be equal
    plt.xlim(min_x, max_x)
    plt.ylim(min_y, max_y)
    plt.savefig("plot.png")  # Save the plot to a file
    plt.close(fig)  # Close the plot

