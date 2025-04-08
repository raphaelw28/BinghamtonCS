class wong_lab3
{
	public static void main(String[] args)
	{
		if(args.length > 2)
			encode(args[0], args[1], args[2].equals("y"));
		else
			encode(args[0],args[1], false);
	}
	public static void encode(String inputFilename, String outputFilename, boolean correct)
	{
		try
		{
			java.io.BufferedReader input = new java.io.BufferedReader(new java.io.InputStreamReader(new java.io.FileInputStream(inputFilename)));
			java.io.PrintWriter output = new java.io.PrintWriter(new java.io.BufferedWriter(new java.io.FileWriter(outputFilename)));
			int lineCounter = 0;
			int wordCounter = 0;
			int charsCounter = 0;
			java.util.ArrayDeque<String> stackOfCurrentStrings = new java.util.ArrayDeque<>();
			java.util.TreeSet<String> uniqueStrings = new java.util.TreeSet<>();
			String inn;
			
			while((inn = input.readLine()) != null)
			{
				String[] text = inn.split("\\s+");
				for(String word : text)
				{
					uniqueStrings.add(word);
					String reversed = "";
					
					char[] reversedArray = new char[word.length()];
					for(int i = 0; i < word.length(); i++)
					{
						reversedArray[i] = word.charAt(word.length() - 1 - i);
						charsCounter++;
					}
					stackOfCurrentStrings.push(reversed);
					wordCounter++;
				}
				String s = "";
				while(stackOfCurrentStrings.size() > 0)
				{
					s += stackOfCurrentStrings.removeLast();
					s += " ";
				}
				s = s.substring(0, s.length() - 1);
				output.println(s);
				lineCounter++;
			}
			input.close();
			output.close();
			System.out.println("lines = " + lineCounter);
			System.out.println("words = " + wordCounter);
			System.out.println("characters in words = " + charsCounter);
			System.out.println("unique words = " + uniqueStrings.size());
			if(correct)
				for(String word : uniqueStrings)
					System.out.println(word);
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
		}
	}
}

