import os
import glob

def generate_xrdcp_for_total_dirs(base_glob, out_dir, remote_prefix="root://your.remote.server"):
    """
    Generate xrdcp commands for all files in `total` subdirs matching a glob pattern.
    
    Args:
        base_glob (str): Glob pattern matching 'total' directories.
        out_dir (str): Base remote output directory.
        remote_prefix (str): Root server prefix.
    """
    created_dirs = set()
    commands = []

    # Get matching 'total' directories
    total_dirs = glob.glob(base_glob)

    for total_dir in total_dirs:
        if not os.path.isdir(total_dir):
            continue

        # Find relative path from ./plots
        rel_path = os.path.relpath(total_dir, "./plots")
        for root, _, files in os.walk(total_dir):
            for file in files:
                local_file = os.path.join(root, file)
                file_rel = os.path.relpath(local_file, "./plots")  # preserve from plots/
                remote_path = os.path.join(out_dir, file_rel)
                remote_dir = os.path.dirname(remote_path)

                if remote_dir not in created_dirs:
                    commands.append(f"xrdfs {remote_prefix} mkdir -p {remote_dir}")
                    created_dirs.add(remote_dir)

                commands.append(f"xrdcp {local_file} {remote_prefix}/{remote_path}")

    return commands

# Example usage
if __name__ == "__main__":
    base_glob = "./plots/UL*/*Assumption/IsoScalarHiggsCuts_Category_90pPrompt_SF_unblinded_250104/total"
    out_dir = "/eos/user/s/sdansana/t2b_bkp/HToSS/HToSS_analysis"  # remote base output path
    remote_server = "root://eosuser.cern.ch"

    cmds = generate_xrdcp_for_total_dirs(base_glob, out_dir, remote_prefix=remote_server)

    with open("xrdcp_total_dirs.sh", "w") as f:
        f.write("#!/bin/bash\n\n")
        for cmd in cmds:
            f.write(cmd + "\n")

    print("Script saved as xrdcp_total_dirs.sh")
