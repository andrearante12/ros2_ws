"""
PyTorch Dataset for imitation learning episodes.

Loads episode_XXXX.json files, each containing:
  observations: list of [13] float lists
  actions: list of [4] float lists
  success: bool

Provides (observation, action) pairs at the timestep level.
Computes and applies normalization (zero mean, unit variance).
"""

import os
import json
import torch
from torch.utils.data import Dataset


class EpisodeDataset(Dataset):
    def __init__(self, episodes_dir, success_only=True, normalize=True, stats=None):
        self.observations = []
        self.actions = []

        # Load all episode files
        files = sorted(f for f in os.listdir(episodes_dir)
                       if f.startswith('episode_') and f.endswith('.json'))

        loaded = 0
        skipped = 0
        for f in files:
            with open(os.path.join(episodes_dir, f)) as fh:
                ep = json.load(fh)
            if success_only and not ep['success']:
                skipped += 1
                continue
            self.observations.append(torch.tensor(ep['observations'], dtype=torch.float32))
            self.actions.append(torch.tensor(ep['actions'], dtype=torch.float32))
            loaded += 1

        if not self.observations:
            raise ValueError(f"No episodes loaded from {episodes_dir} "
                             f"(found {len(files)} files, skipped {skipped} failures)")

        self.observations = torch.cat(self.observations, dim=0)
        self.actions = torch.cat(self.actions, dim=0)

        print(f"Loaded {loaded} episodes ({skipped} skipped), "
              f"{len(self.observations)} timesteps")

        # Normalization
        if stats is not None:
            self.obs_mean, self.obs_std = stats['obs_mean'], stats['obs_std']
            self.act_mean, self.act_std = stats['act_mean'], stats['act_std']
        else:
            self.obs_mean = self.observations.mean(dim=0)
            self.obs_std = self.observations.std(dim=0).clamp(min=1e-6)
            self.act_mean = self.actions.mean(dim=0)
            self.act_std = self.actions.std(dim=0).clamp(min=1e-6)

        self.normalize = normalize

    def get_stats(self):
        return {
            'obs_mean': self.obs_mean,
            'obs_std': self.obs_std,
            'act_mean': self.act_mean,
            'act_std': self.act_std,
        }

    def __len__(self):
        return len(self.observations)

    def __getitem__(self, idx):
        obs = self.observations[idx]
        act = self.actions[idx]
        if self.normalize:
            obs = (obs - self.obs_mean) / self.obs_std
            act = (act - self.act_mean) / self.act_std
        return obs, act
