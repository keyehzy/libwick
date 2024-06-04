#!/usr/bin/env python3

import os
import subprocess
import requests

TOKEN = os.getenv("GITHUB_TOKEN")
OWNER = os.getenv("GITHUB_OWNER")
REPO = os.getenv("GITHUB_REPOSITORY")
EXTENSIONS = (".cpp", ".h")
DIRECTORIES = ["src", "tests", "examples"]


def create_github_issue(title, body):
    url = f"https://api.github.com/repos/{OWNER}/{REPO}/issues"
    headers = {
        "Authorization": f"token {TOKEN}",
        "Accept": "application/vnd.github.v3+json",
    }
    data = {"title": title, "body": body, "labels": ["TODO"], "assignees": [OWNER]}
    response = requests.post(url, headers=headers, json=data)
    if response.status_code == 201:
        print(f"Successfully created issue: {title}")
    else:
        raise RuntimeError(f"Error: Could not create issue: {response.content}")


def github_current_issues():
    url = f"https://api.github.com/repos/{OWNER}/{REPO}/issues"
    headers = {
        "Authorization": f"token {TOKEN}",
        "Accept": "application/vnd.github.v3+json",
    }
    params = {"state": "open", "labels": "TODO"}
    response = requests.get(url, headers=headers, params=params)
    if response.status_code == 200:
        issues = response.json()
        return [issue["title"] for issue in issues]
    else:
        raise RuntimeError(f"Error: Could not fetch issues: {response.content}")


def find_todos():
    git_toplevel = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"], shell=False, capture_output=True
    )

    if git_toplevel.returncode != 0:
        raise RuntimeError("Error: Not a git repository")

    directory = git_toplevel.stdout.decode("utf-8").strip()
    todos = []

    for dir in DIRECTORIES:
        current_directory = os.path.join(directory, dir)
        for root, _, files in os.walk(current_directory):
            for file in files:
                if file.endswith(EXTENSIONS):
                    file_path = os.path.join(root, file)
                    with open(file_path, "r") as f:
                        for i, line in enumerate(f):
                            if "TODO" in line:
                                path_sans_root = file_path[len(directory) :]
                                todo_offset = line.index("TODO")
                                lines_sans_prefix = line[todo_offset:].strip()
                                todos.append((path_sans_root, i + 1, lines_sans_prefix))
    return todos


if __name__ == "__main__":
    if not TOKEN or not OWNER or not REPO:
        raise RuntimeError(
            "Error: GITHUB_TOKEN, GITHUB_OWNER and GITHUB_REPOSITORY must be set"
        )

    todos = find_todos()
    issues = github_current_issues()

    for todo in todos:
        file_path, line_number, line = todo
        title = line.strip()
        body = f"{file_path}:{line_number}\n"
        if title not in issues:
            create_github_issue(title, body)
        else:
            print(f"Skipping issue, already exists:\n  {title}")

# Copyright (c) 2024 Matheus Sousa
# SPDX-License-Identifier: BSD-2-Clause
