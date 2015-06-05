package rjr;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;

public class ChallangesManager {

	private ChallangesManager() {
		this._challangeResultsByMap = new HashMap<String, SortedMap<String, String>>();
	}

	private static ChallangesManager _instance = null;

	public static ChallangesManager Instance() {
		if (ChallangesManager._instance == null)
			ChallangesManager._instance = new ChallangesManager();

		return ChallangesManager._instance;
	}

	private Map<String, SortedMap<String, String>> _challangeResultsByMap = null;

	public ChallangeResult submitChallangeResult(String mapid, String playerid,
			String time) {
		if (this._challangeResultsByMap.containsKey(mapid) == false) {
			this._challangeResultsByMap.put(mapid,
					new TreeMap<String, String>());
		}
		this._challangeResultsByMap.get(mapid).put(time, playerid);

		ChallangeResult result = new ChallangeResult();

		result.setPlayerId(playerid);
		result.setTime(time);
		result.setPlayerName(playerid);

		return result;
	}

	public ArrayList<ChallangeResult> getTop5Results(String mapid) {
		ArrayList<ChallangeResult> result = new ArrayList<ChallangeResult>();

		if (this._challangeResultsByMap.containsKey(mapid)) {
			int i = 0;
			for (SortedMap.Entry<String, String> entry : this._challangeResultsByMap
					.get(mapid).entrySet()) {
				if (i < 5) {
					ChallangeResult c = new ChallangeResult();
					c.setPlayerId(entry.getValue());
					c.setPlayerName(entry.getValue());
					c.setTime(entry.getKey());
					result.add(c);
				} else {
					break;
				}
			}
		}

		return result;
	}

	public Map<String, ArrayList<ChallangeResult>> getAllTop5Results() {
		SortedMap<String, ArrayList<ChallangeResult>> result = new TreeMap<String, ArrayList<ChallangeResult>>();

		for (Map.Entry<String, SortedMap<String, String>> map : this._challangeResultsByMap.entrySet()) {
			result.put(map.getKey(), new ArrayList<ChallangeResult>());
			int i = 0;
			for (SortedMap.Entry<String, String> entry : map.getValue().entrySet()) {
				if (i < 5) {
					ChallangeResult c = new ChallangeResult();
					c.setPlayerId(entry.getValue());
					c.setPlayerName(entry.getValue());
					c.setTime(entry.getKey());
					result.get(map.getKey()).add(c);
				} else {
					break;
				}
			}
		}
		return result;
	}

}
