class wong_p2 
{
    public static void main(String[] args)
    {
        if(args.length < 3)
        {
            System.out.println("Three Commandline Arguments are not Provided");
            System.exit(0);

        }
            if (args[0].equals("b2t"))
                {
                    convertBinaryToText(args[1], args[2]);
                }
            else if (args[0].equals("t2b"))
                {
                    convertTextToBinary(args[1], args[2]);
                }
            else 
                {  
                    System.out.println("Incorrect Commandline Argument");
                    System.exit(0);
                }
    }

    public static void convertTextToBinary(String inputFileName, String outputFileName)
    {
        try
        {
            java.io.BufferedReader input = new java.io.BufferedReader(new java.io.InputStreamReader(new java.io.FileInputStream(inputFileName)));
            java.io.DataOutputStream output = new java.io.DataOutputStream(new java.io.FileOutputStream(outputFileName));

            int lines = 0;
            while (input.readLine() != null)
                lines++;
            output.writeInt(lines);

            String inn;
            while((inn = input.readLine()) !=null)
            {
                String[] token = inn.split("\t");

                if(token[0].equals("string"))
                {
                    output.writeChars("s");
                    output.writeInt(token[1].length());
                    for(int i = 0; i < token[1].length(); i++)
                        output.writeChar(token[1].charAt(i));
                }
            
                if(token[0].equals("double")) 
                {
                    output.writeChars("d");
                    output.writeDouble(Double.parseDouble(token[1]));
                }

                if(token[0].equals("int")) 
                {
                    output.writeChars("i");
                    output.writeInt(Integer.parseInt(token[1]));
                }

                if(token[0].equals("float")) 
                {
                    output.writeChars("f");
                    output.writeFloat(Float.parseFloat(token[1]));
                }

                if(token[0].equals("long")) 
                {
                    output.writeChars("l");
                    output.writeLong(Long.parseLong(token[1]));
                }

                if(token[0].equals("short"))
                {
                    output.writeChars("h");
                    output.writeShort(Short.parseShort(token[1]));
                }

                if(token[0].equals("long array")) 
                {
                    output.writeChars("b");
                    String[] values = token[1].split(",");
                    output.writeInt(values.length);
                    for(int i = 0; i < values.length; i++)
                        output.writeLong(Long.parseLong(values[i]));                
                }

                if(token[0].equals("float array")) 
                {
                    output.writeChars("g");
                    String[] values = token[1].split(",");
                    output.writeInt(values.length);
                    for(int i = 0; i < values.length; i++)
                        output.writeFloat(Float.parseFloat(values[i]));
                }
            }
            input.close();
            output.close();
        }
        catch (Exception e)
        {
            System.out.println(e.toString());
            System.exit(0);
        }
    }

    public static void convertBinaryToText(String inputFileName, String outputFileName)
    {
        try
        {
            java.io.DataInputStream input = new java.io.DataInputStream(new java.io.FileInputStream(inputFileName));
            java.io.PrintWriter output = new java.io.PrintWriter(outputFileName);

            int lines = input.readInt();
            for(int i = 0; i < lines; i++)
            {
                char c = input.readChar();
                int length = input.readInt();


                switch(c)
                {
                    case 'i':
                        output.print("int");
                        output.print("\t");
                        output.print(input.readInt());
                        break;
                    
                    case 'l':
                        output.print("long");
                        output.print("\t");
                        output.print(input.readLong());
                        break;

                    case 'h':
                        output.print("short");
                        output.print("\t");
                        output.print(input.readShort());
                        break;
                    
                    case 'f':
                        output.print("float");
                        output.print("\t");
                        output.print(input.readFloat());
                        break;
                    
                    case 'd':
                        output.print("double");
                        output.print("\t");
                        output.print(input.readDouble());
                        break;

                    case 'b':
                        output.print("long array");
                        output.print("\t");
                        for(int j = 0; j < length; j++)
                        {
                            output.print(input.readLong());
                            if(j < length-1)
                                output.print(",");
                        }   
                        break;

                    case 's':
                        output.print("string");
                        output.print("\t");
                        for(int j = 0; j < length; j++)
                            output.print(input.readChar());
                        break;

                    case 'g':
                        output.print("float array");
                        output.print("\t");
                        for(int j = 0; j < length; j++)
                        {
                            output.print(input.readFloat());
                            if(j < length-1)
                                output.print(",");
                        }
                        break;
                    
                    default:
                        break;
                }

            }
            input.close();
            output.close();
        }
        catch (Exception e)
        {
            System.out.println(e.toString());
            System.exit(0);
        }
    }
}
