"""
Behavioral cloning policy network.

Input:  13D observation (7 joint pos + 3 EE pos + 3 object pos)
Output: 4D action (3 delta-XYZ + 1 gripper logit)

The network outputs normalized actions during training.
At deployment, actions are denormalized using saved stats.
"""

import torch
import torch.nn as nn


class BCPolicy(nn.Module):
    def __init__(self, obs_dim=13, act_dim=4, hidden=256):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(obs_dim, hidden),
            nn.ReLU(),
            nn.Linear(hidden, hidden),
            nn.ReLU(),
            nn.Linear(hidden, act_dim),
        )

    def forward(self, obs):
        return self.net(obs)


def load_policy(checkpoint_path, device='cpu'):
    """Load a trained policy + normalization stats from a checkpoint."""
    ckpt = torch.load(checkpoint_path, map_location=device, weights_only=False)
    policy = BCPolicy(
        obs_dim=ckpt.get('obs_dim', 13),
        act_dim=ckpt.get('act_dim', 4),
        hidden=ckpt.get('hidden', 256),
    )
    policy.load_state_dict(ckpt['model'])
    policy.eval()
    stats = ckpt['stats']
    return policy, stats
