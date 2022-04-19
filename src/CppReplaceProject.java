
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.io.FileWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CppReplaceProject {
	private static String slotPath = "/";
	private static String forderName = "/";
	private static String className = "CSlot346PiggyAdventure";
	private static String path = slotPath + forderName + className + ".cpp";
	private static String testPath = "/Users/dg-2102-pn-002/work/mobile/cppReplaceProject/src/test.cpp";
	private static List<String> commonString = new <String>ArrayList();

	public static void main(String[] args) {
		addCommonString();

		int mode = 1;
		switch (mode) {
			case 1:
				replaceJsToCpp();
				break;
			case 2:
				cppFunctionExtract();
				break;
			case 3:
				break;
			default:
				break;
		}
	}

	private static void replaceJsToCpp() {
		String text;
		try {
			text = new String(Files.readAllBytes(Paths.get(testPath)), StandardCharsets.UTF_8);
			// text = new String(Files.readAllBytes(Paths.get(path)), StandardCharsets.UTF_8);
			//함수{}
			text = text.replaceAll("\\n([\\w]*\\(.*\\))\\n", "\nvoid "+ className +"::$1\n");

			//기타
			text = text.replaceAll("\\= null;", "\\= nullptr;");
			text = text.replaceAll("== =", "==");
			text = text.replaceAll("!= =", "!=");
			text = text.replaceAll("this.\\$slot\\.", "");
			text = text.replaceAll("this\\.", "");
			text = text.replaceAll("\\.visible = (.*);", "->setVisible($1);");
			text = text.replaceAll("super\\.", "super::");
			text = text.replaceAll("CMiscUtils\\.", "CMiscUtils::");
			text = text.replaceAll("ArrayUtils\\.", "ArrayUtils::");
			text = text.replaceAll("CAnimateSlotBase\\.", "CAnimateSlotBase::");
			text = text.replaceAll("SPIN_MODE\\.", "SPIN_MODE::");
	
			text = text.replaceAll("(let|var) ", "auto ");
			text = text.replaceAll("\\.length", "\\.size()");
			text = text.replaceAll("; (.*) < (.*).size\\(\\)", ",size = (int)$2.size(); $1 < size");
			text = text.replaceAll("\\.alpha = (.*);", "->setOpacity(255 * $1);");
			text = text.replaceAll("\\.scale\\.x = (.*);", "->setScaleX($1);");
			text = text.replaceAll("\\.scale\\.y = (.*);", "->setScaleY($1);");
			text = text.replaceAll("\\.x = (.*);", "->setPositionX($1);");
			text = text.replaceAll("\\.y = (.*);", "->setPositionY($1);");
			//a -= 1 ,a += 1 형태 --> = a + 1로 바꾸고 실행
			text = text.replaceAll("\\.x", "->getPositionX()");
			text = text.replaceAll("\\.y", "->getPositionY()");
			text = text.replaceAll("\\.push\\((.*)\\);", ".push_back($1);");
			text = text.replaceAll("Math\\.floor\\((.*)\\)", "$1");
			text = text.replaceAll("(onClickSpin\\(e\\)|onClickSpin\\(\\))", "onClickSpinBtn()");
			text = text.replaceAll("(onClickBetUp\\(e\\)|onClickBetUp\\(\\))", "onClickBetPlusBtn()");
			text = text.replaceAll("(onClickBetDown\\(e\\)|onClickBetDown\\(\\))", "onClickBetMinusBtn()");
			text = text.replaceAll("(onClickMaxBet\\(e\\)|onClickMaxBet\\(\\))", "onClickMaxbetBtn()");
			text = text.replaceAll("(onClickPaytable\\(e\\)|onClickPaytable\\(\\))", "onClickPaytableBtn()");
			// text = text.replaceAll("_isEnableAuto\\(\\)", "IsCanAutoRun()");
			text = text.replaceAll("setSpinAgain", "SetSpinAgain");
			text = text.replaceAll("SetGoodLuck", "SetGoodLuck");
			text = text.replaceAll("bonuspot_number = (.*);", "set_bonuspot_number($1);");
			//_featuredMap변수 자신이 사용하는 변수명으로 바꿔줄 것
			// text = text.replaceAll("_featuredMap\\.([\\w]*)", "_featuredMap->HasMember(\"$1\")");
			// text = text.replaceAll("", "");
			// text = text.replaceAll("", "");
			// text = text.replaceAll("", "");
			// text = text.replaceAll("", "");
			// text = text.replaceAll("", "");
			// text = text.replaceAll("", "");
			// text = text.replaceAll("", "");
			for (int i = 0; i < commonString.size(); i++) {
				text = text.replaceAll(commonString.get(i), commonString.get(i).replaceAll("\\.", "->"));
			}

			//디렉션
			text = text.replaceAll("_directionController->addStepFunction\\([\\n ]*(.*),.*[\\n ]*.*\\((.*)\\);", "_directionController->addStepFunc($1,[=](){_directionController->makeSmallSteps($2,\"\")");

			//bottom
			text = text.replaceAll("\\$bottomUI", "m_bottomPanel");
			text = text.replaceAll("m_bottomPanel\\.", "m_bottomPanel->");

			//co (bind옵션 사라짐)
			text = text.replaceAll("addAnimationComplete.*[\\n ]*(.*), \\(.*\\) = > \\{", "addAnimationComplete($1,[this](string entry) {");
			text = text.replaceAll("co\\(function \\* \\(\\) \\{[\\n ]*try \\{", "coFunctionWait(0, [this]{");
			text = text.replaceAll("coFunctionWait.*[\\n ]*yield wait\\((.*)\\);", "coFunctionWait($1, [this] {");
			text = text.replaceAll("yield wait\\((.*)\\);", "coFunctionWait($1, [this] {});//wait");
			text = text.replaceAll("\\}[\\n ]*catch.*[\\n ]*.*[\\n ]*\\}[\\n ]*\\}.bind.*\\n", "});\n");

			//string format
			text = text.replaceAll("`\\$ \\{ ([a-zA-Z0-9_+ -*/%]*) \\}[\\n ]*`", "StringUtils::toString($1)");
			text = text.replaceAll("`([a-zA-Z0-9_]*)\\$.*\\{([a-zA-Z0-9_+ -*/%]*)\\}[ ]*([a-zA-Z0-9_]*)[\\n ]*`", "StringUtils::format(\"$1%d$3\",$2)");
			text = text.replaceAll("`\\$.*\\{([a-zA-Z0-9_+ -*/%]*)\\}[ ]*([a-zA-Z0-9_]*)[\\n ]*`", "StringUtils::format(\"%d$2\",$1)");

			//font
			text = text.replaceAll("\\.text = (.*);", "->setString($1);");
			text = text.replaceAll("\\.setAutoAlign\\(\\);", "->setAnchorPoint(ccp(0.5, 0.5));");

			//removeChild
			text = text.replaceAll("\\.parent\\.removeChild.*;", "->removeFromParent();");
			text = text.replaceAll("\n .*\\.destroy.*;\n", "\n");

			//개인별 수정사항
			// text = text.replaceAll(" \\&\\& .*\\.parent", "");
			// text = text.replaceAll("\n.*_is_destroy.*[\\n ]*.*[ \\n]*\\}\\n", "\n");
			// text = text.replaceAll("(.*)->adjustPosition\\((.*)\\);", "customAdjustPosition($1,$2);");
			// text = text.replaceAll("([\\w \\[\\]]*)\\.split\\(\"(.*)\"\\)", "Split($1,'$2')");

			//기타
			text = text.replaceAll("\'", "\"");
			text = text.replaceAll("\\$", "_");

			File file = new File("./temp.cpp");
			BufferedWriter writer = new BufferedWriter(new FileWriter(file));
			writer.write(text);
			writer.close();
		}
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	//함수뽑기 
	private static void cppFunctionExtract() {
		try {
			File file = new File(slotPath + forderName + className + ".cpp");
			if (file.exists()) {
				BufferedReader inFile = new BufferedReader(new FileReader(file));
				String sLine = null;
				Pattern pattern = Pattern.compile("^(void|int|CSlotReelWrapper*|bool|float|CSimpleCCBI*) " + className + "::(.*)");
				Matcher matcher;
				while ((sLine = inFile.readLine()) != null) {
					matcher = pattern.matcher(sLine);
					if (matcher.find()) {
						System.out.println(matcher.group().replaceAll(className + "::", "") + ";");
					}
				}
			}
		}
		catch (FileNotFoundException e) {
			e.getStackTrace();
		}
		catch (IOException e) {
			e.getStackTrace();
		}
	}

	private static void addCommonString(){
		commonString.add("\\.setAnimation");
		commonString.add("\\.borrow");
		commonString.add("\\.addAnimation");
		commonString.add("\\.addChild");
		commonString.add("\\.addAsyncLinkSpine");
		commonString.add("\\.adjustPosition");
		commonString.add("\\.freespinTotalAward");
		commonString.add("\\.addBitmapText");
		commonString.add("\\.gotoAndStop");
		commonString.add("\\.playLines");
		commonString.add("\\._dug_icon_id");
		commonString.add("reel\\.");
		commonString.add("slotReelWrapper\\.");
		commonString.add("currentResult\\.");
		commonString.add("spinResult\\.");
		commonString.add("payout\\.");
		commonString.add("freespin\\.");
		commonString.add("icon\\.");
		commonString.add("slotInfo\\.");
		commonString.add("_slotLoginInfo\\.");
		commonString.add("_directionController\\.");
		commonString.add("aIcon\\.");
		commonString.add("spinProcessQueue\\.");
	}
}
