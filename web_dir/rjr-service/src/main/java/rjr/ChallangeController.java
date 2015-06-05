package rjr;

import java.util.ArrayList;
import java.util.Map;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class ChallangeController {

	@RequestMapping(value = "/challange/submit", method = RequestMethod.POST)
	public ChallangeResult submit(
			@RequestParam(value = "mapid", defaultValue = "default") String mapid,
			@RequestParam(value = "playerid", defaultValue = "default") String playerid,
			@RequestParam(value = "time", defaultValue = "999999") String time) {

		return ChallangesManager.Instance().submitChallangeResult(mapid,
				playerid, time);
	}

	@RequestMapping("/challange/top")
	public ArrayList<ChallangeResult> challangeTop(
			@RequestParam(value = "mapid", defaultValue = "default") String mapid) {

		return ChallangesManager.Instance().getTop5Results(mapid);
	}

	@RequestMapping("/challange/all")
	public Map<String, ArrayList<ChallangeResult>> allChallangesTop() {

		return ChallangesManager.Instance().getAllTop5Results();
	}
}
