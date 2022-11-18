import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def animate(time_ray, data_grid, V_grid, name, cmap="viridis", renormalize=False):
    # Create figure
    fig = plt.figure(tight_layout=True)
    ax = plt.gca()

    # Create a colour scale normalization according to the max z value in the first frame
    norm = matplotlib.cm.colors.Normalize(vmin=np.min(data_grid[0]), vmax=np.max(data_grid[0]))

    # Plot the first frame
    img = ax.imshow(data_grid[0], extent=[0,1,0,1], cmap=cmap, norm=norm)
    ax.imshow(V_grid, extent=[0,1,0,1], cmap="gray")

    plt.xlabel("x []")
    plt.ylabel("y []")

    cbar = fig.colorbar(img, ax=ax)
    cbar.set_label("z(x,y,t)")

    # Add a text element showing the time
    time_txt = plt.text(0.95, 0.95, f"t = {time_ray[0]:.5f}", color="white", horizontalalignment="right", verticalalignment="top")

    # Function that takes care of updating the z data and other things for each frame
    def animation(i):
        print(f"\r{i+1}/{len(time_ray)}", end="")
        # Normalize the colour scale to the current frame?
        if renormalize:
            norm = matplotlib.cm.colors.Normalize(vmin=np.min(data_grid[0]), vmax=np.max(data_grid[i]))
            img.set_norm(norm)

        # Update z data
        img.set_data(data_grid[i])

        # Update the time label
        time_txt.set_text(f"t = {time_ray[i]:.5f}")

        return img

    anim = FuncAnimation(fig, animation, interval=1, frames=np.arange(0, len(time_ray), 1), repeat=False, blit=0)
    anim.save(name, writer="ffmpeg", fps=30, dpi=200)
    print()
