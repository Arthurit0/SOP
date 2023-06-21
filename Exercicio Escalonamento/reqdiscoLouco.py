def read_requests():
    requests = []
    try:
        while True:
            request = input().strip().split()
            if request:
                requests.append((int(request[0]), int(request[1]), request[2]))
    except EOFError:
        pass
    return requests


def merge_requests(requests):
    merged = []
    for req in sorted(requests):
        if (
            not merged
            or req[2] != merged[-1][2]
            or req[0] != merged[-1][0] + merged[-1][1]
            or merged[-1][1] + req[1] > 64
        ):
            merged.append(req)
        else:
            new_size = min(merged[-1][1] + req[1], 64)
            merged[-1] = (merged[-1][0], new_size, merged[-1][2])
    return merged


def main():
    requests = read_requests()
    merged_requests = merge_requests(requests)
    print("Fila:")
    for req in merged_requests:
        print(req[0], req[1], req[2])


if __name__ == "__main__":
    main()
