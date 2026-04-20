import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Simulate 32 threads accessing shared memory (banked)
# Banks: 0-31
# Conflict: Threads accessing same bank
# Each bank will be a square, with thread lines drawn to banks

def draw_bank_conflict(conflict=True):
    fig, ax = plt.subplots(figsize=(12, 4))
    ax.set_xlim(0, 33)
    ax.set_ylim(0, 5)
    ax.axis('off')

    # Draw memory banks
    for i in range(32):
        bank = patches.Rectangle((i, 2), 1, 1, edgecolor='black', facecolor='lightgray')
        ax.add_patch(bank)
        ax.text(i + 0.5, 2.5, f"B{i}", ha='center', va='bottom', fontsize=8)

    # Draw threads
    for tid in range(32):
        ax.text(tid + 0.5, 4.5, f"T{tid}", ha='center', va='bottom', fontsize=6)
        ax.plot([tid + 0.5, (tid * 2) % 32 + 0.5], [4.3, 2], color='red' if conflict and ((tid * 2) % 32) % 16 == 0 else 'green')

    title = "Shared Memory Bank Access Pattern: " + ("❌ With Conflict" if conflict else "✅ Without Conflict (Padded)")
    ax.set_title(title, fontsize=14)
    plt.tight_layout()
    plt.show()

draw_bank_conflict(conflict=True)  # With bank conflict

