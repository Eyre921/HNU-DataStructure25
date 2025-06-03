import sys

def solve():
    N, M = map(int, sys.stdin.readline().split())
    A = list(map(int, sys.stdin.readline().split()))

    def can_divide(max_sum_allowed):
        num_segments = 1
        current_segment_sum = 0
        for x_val in A:
            if x_val > max_sum_allowed:
                return False  # Single element is too large

            if current_segment_sum + x_val <= max_sum_allowed:
                current_segment_sum += x_val
            else:
                num_segments += 1
                current_segment_sum = x_val

        return num_segments <= M

    # N is positive, so A is not empty.
    # Smallest possible answer is the largest single element.
    # If A_i can be 0 and all are 0, max(A) is 0.
    # The problem states "正整数数列 A" (positive integers) and "输出正整数" (output positive integer),
    # implying inputs will result in a positive answer.
    # Thus, max(A) >= 1.
    low = 0
    if N > 0: # N is positive integer
        # Iterate to find max element if necessary, or use Python's max()
        # Assuming A is not empty as per N >= 1
        max_val_in_A = A[0]
        for i in range(1,N):
            if A[i] > max_val_in_A:
                max_val_in_A = A[i]
        low = max_val_in_A

    # Largest possible answer is the sum of all elements.
    high = 0
    if N > 0: # Assuming A is not empty
        current_sum = 0
        for x in A:
            current_sum += x
        high = current_sum

    ans = high

    while low <= high:
        mid = low + (high - low) // 2
        if mid == 0 and high == 0: # Special case for all A_i = 0, if allowed
            if can_divide(0):
                ans = 0
                break
            else: # Should not happen if all A_i are 0
                low = 1 # Avoid infinite loop if mid = 0 leads to low = 0+1, high remains 0
                continue

        if can_divide(mid):
            ans = mid
            high = mid - 1
        else:
            low = mid + 1

    print(ans)

solve()
