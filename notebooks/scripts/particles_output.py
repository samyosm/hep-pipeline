import matplotlib.pyplot as plt
import numpy as np
import uproot

file = uproot.open("data/detector_simulation.root")
tree = file["Steps"]

bins = 200
H = np.zeros((bins, bins), dtype=np.float64)

for batch in tree.iterate(["X", "Y", "Edep"], library="np", step_size="500 MB"):
    H += np.histogram2d(batch["X"], batch["Y"], bins=bins, weights=batch["Edep"])[0]

plt.imshow(H.T, aspect="equal", origin="lower")
plt.colorbar(label="Energy deposit")
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Detector energy map (XY)")
plt.show()
