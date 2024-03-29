package suffixarray_lcparray;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Suffix Array 정렬 알고리즘으로 생성하기
 * 값 비교 O(n)
 * 값 정렬 O(nlgn)
 * 시간복잡도 O(n^2*lgn)
 */

public class SuffixArray_Sort {
	public static void main(String[] args) {
		String text = "alohomora";
		List<Integer> suffix = getSuffixArray(text);

		for(int i : suffix){
			System.out.println(i + " - " + text.substring(i));
		}
	}

	private static List<Integer> getSuffixArray(String text) {
		int n = text.length();
		// 접미사 배열이 될 반환 값
		List<Integer> perm = new ArrayList<>();
		for (int i = 0; i < n; i++) {
			perm.add(i);
		}
		Collections.sort(perm, Comparator.comparing(text::substring));
		return perm;
	}

}

