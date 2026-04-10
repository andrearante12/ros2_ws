"""
Behavioral cloning training script.

Trains an MLP policy on recorded human demonstrations.

Usage:
  python3 -m moveit_controls.il.train
  python3 -m moveit_controls.il.train --episodes_dir ~/demonstrations/episodes
  python3 -m moveit_controls.il.train --epochs 300 --lr 3e-4
"""

import os
import argparse

import torch
import torch.nn as nn
from torch.utils.data import DataLoader, random_split

from moveit_controls.il.dataset import EpisodeDataset
from moveit_controls.il.policy import BCPolicy


def train(args):
    device = torch.device('cpu')

    # Load dataset
    dataset = EpisodeDataset(args.episodes_dir, success_only=True, normalize=True)
    stats = dataset.get_stats()

    # Train/val split
    n_val = max(1, int(len(dataset) * 0.1))
    n_train = len(dataset) - n_val
    train_set, val_set = random_split(dataset, [n_train, n_val])

    train_loader = DataLoader(train_set, batch_size=args.batch_size, shuffle=True)
    val_loader = DataLoader(val_set, batch_size=args.batch_size)

    print(f"Train: {n_train}, Val: {n_val}")

    # Model
    policy = BCPolicy(obs_dim=13, act_dim=4, hidden=args.hidden).to(device)
    optimizer = torch.optim.Adam(policy.parameters(), lr=args.lr)

    # Loss: MSE on all 4 outputs (delta-xyz + gripper)
    # Both are normalized, so MSE works for both continuous and binary targets
    criterion = nn.MSELoss()

    # Training loop
    best_val_loss = float('inf')
    os.makedirs(args.output_dir, exist_ok=True)
    checkpoint_path = os.path.join(args.output_dir, 'best.pt')

    for epoch in range(1, args.epochs + 1):
        # Train
        policy.train()
        train_loss = 0.0
        for obs, act in train_loader:
            obs, act = obs.to(device), act.to(device)
            pred = policy(obs)
            loss = criterion(pred, act)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            train_loss += loss.item() * obs.size(0)
        train_loss /= n_train

        # Validate
        policy.eval()
        val_loss = 0.0
        with torch.no_grad():
            for obs, act in val_loader:
                obs, act = obs.to(device), act.to(device)
                pred = policy(obs)
                loss = criterion(pred, act)
                val_loss += loss.item() * obs.size(0)
        val_loss /= n_val

        if epoch % 10 == 0 or epoch == 1:
            print(f"Epoch {epoch:4d}/{args.epochs}  train_loss={train_loss:.6f}  val_loss={val_loss:.6f}")

        if val_loss < best_val_loss:
            best_val_loss = val_loss
            torch.save({
                'model': policy.state_dict(),
                'stats': stats,
                'obs_dim': 13,
                'act_dim': 4,
                'hidden': args.hidden,
                'epoch': epoch,
                'val_loss': val_loss,
            }, checkpoint_path)

    print(f"\nBest val_loss: {best_val_loss:.6f}")
    print(f"Checkpoint saved to: {checkpoint_path}")


def main():
    parser = argparse.ArgumentParser(description='Train BC policy for cube lift')
    parser.add_argument('--episodes_dir', type=str,
                        default=os.path.expanduser('~/demonstrations/episodes'))
    parser.add_argument('--output_dir', type=str,
                        default=os.path.expanduser('~/models/bc_cube_lift'))
    parser.add_argument('--epochs', type=int, default=200)
    parser.add_argument('--batch_size', type=int, default=64)
    parser.add_argument('--lr', type=float, default=1e-4)
    parser.add_argument('--hidden', type=int, default=256)
    args = parser.parse_args()
    train(args)


if __name__ == '__main__':
    main()
