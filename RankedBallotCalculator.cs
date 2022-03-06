using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;

namespace RankedBallotSystem
{
    // class that handles ballot calculations
    class RankedBallotCalculator
    {
        // tracks total number of votes
        public int totalVotes = 0;
        // tracks the lowest amount of votes among candidates not including zero
        public int lowestVotes = 1;
        // tracks if a winner has been declared, is used in main loop
        bool winnerDeclared = false;
        // predicate for eliminating candidates with the lowest number of votes (non-zero)
        private static bool hasLowestVotes(Candidate candidate)
        {
            return candidate.hasLowestVoteNum;
        }
        // predicate for eliminating candidates with zero votes
        private static bool hasZeroVotes(Candidate candidate)
        {
            return candidate.getVotes() == 0;
        }

        class Candidate
        {
            // flag which signifies whether or not this candidate has the lowest number of votes (non-zero)
            public bool hasLowestVoteNum;
            // First and Last name
            private string candidateName;
            // number of votes
            private int votes;
            // flag which signifies whether or not this candidate has been eliminated from voting, and thus be excluded from calculations
            public bool eliminatedFromPlay;
            // getter for name
            public string getName()
            {
                return candidateName;
            }
            // getter for votes
            public int getVotes()
            {
                return votes;
            }
            // function to increment votes by 1
            public void addVote()
            {
                votes++;
            }
            // function to reset votes (used between rounds)
            public void resetVotes()
            {
                votes = 0;
            }
            // Candidate constructor
            public Candidate(string candidateName, int votes)
            {
                this.candidateName = candidateName;
                this.votes = votes;
                this.hasLowestVoteNum = false;
                this.eliminatedFromPlay = false;
        }
        }
        // RankedBallotCalculator method that reads through a given CSV file
        public static DataTable ReadCSV(DataTable myTable, string filepath)
        {
                var lines = File.ReadAllLines(filepath, System.Text.Encoding.UTF7);
                for (int i = 0; i < lines.Length; i++)
                {
                    if (i != 0)
                    {
                        myTable.Rows.Add(lines[i].Split(','));
                    }
                }
            return myTable;
        }
        static void Main(string[] args)
        {
            //flag that is true while the program is running
            bool inUse = true;
            //outer main loop
            while (inUse)
            {
                //flag for file loading loop, true ends the loop
                bool fileLoaded = false;
                // instantiating number of candidates eliminated
                int numEliminated = 0;
                // calculator object instantiation
                RankedBallotCalculator myCalculator = new RankedBallotCalculator();
                // candidate name list (used for constructing Candidate class list later on)
                List<string> candidatesList = new List<string>();
                // data table construction
                DataTable myTable = new DataTable("DataTable");
                myTable.Columns.Add(new DataColumn("VoterID", typeof(string)));
                myTable.Columns.Add(new DataColumn("firstChoice", typeof(string)));
                myTable.Columns.Add(new DataColumn("secondChoice", typeof(string)));
                myTable.Columns.Add(new DataColumn("thirdChoice", typeof(string)));
                // file loading loop
                while (!fileLoaded)
                {
                    // holds file name
                    string fileName;
                    // user prompt
                    Console.WriteLine("Please enter the name of the ballot file (including the extension): ");
                    fileName = Console.ReadLine();
                    // path grabbed
                    string myPath = Path.GetFullPath(fileName);
                    // file reading function called
                    myTable = ReadCSV(myTable, myPath);

                    //loop adding candidates to the string list
                    foreach (DataRow myRow in myTable.Rows)
                    {
                        string sVoterID = myRow["VoterID"].ToString().Trim();
                        string sfirstChoice = myRow["firstChoice"].ToString().Trim();
                        if (sfirstChoice != string.Empty)
                            candidatesList.Add(sfirstChoice);
                        string ssecondChoice = myRow["secondChoice"].ToString().Trim();
                        if (ssecondChoice != string.Empty)
                            candidatesList.Add(ssecondChoice);
                        string sthirdChoice = myRow["thirdChoice"].ToString().Trim();
                        if (sthirdChoice != string.Empty)
                            candidatesList.Add(sthirdChoice);
                        //Console.WriteLine(sVoterID);
                        //Console.WriteLine(sfirstChoice);
                        //Console.WriteLine(ssecondChoice);
                        //Console.WriteLine(sthirdChoice);
                    }
                    //Prompt user to add more files    
                    Console.WriteLine("");
                    Console.WriteLine("Would you like to add another file? (yes/no)");
                    string moreFiles = Console.ReadLine();
                    if (moreFiles != "yes")
                    {
                        fileLoaded = true;
                    }
                }

                //user chooses name/file for text output
                Console.WriteLine("Please enter a filename for output (with or without an extension): ");
                string outputName = Console.ReadLine();
                //string outputPath = Path.GetFullPath(outputName);
                FileStream myStream = new FileStream(outputName, FileMode.Create, FileAccess.Write);
                StreamWriter myWriter = new StreamWriter(myStream);
                // Creates list of unique candidate names
                candidatesList = candidatesList.Distinct<string>().ToList();

                // Initializes candidate struct list, and fills it with candidate names and vote amounts (0)
                List<Candidate> structList = new List<Candidate>();
                foreach (string candidate in candidatesList)
                {
                    structList.Add(new Candidate(candidate, 0));
                }
                // tracks round number
                int round = 0;
                // inner main loop that only stops upon a winner being declared
                while (!myCalculator.winnerDeclared)
                {
                    round++;
                    Console.Write("Round ");
                    Console.WriteLine(round);
                    if (round >= 2)
                    {
                        // votes are reset after each round
                        myCalculator.totalVotes = 0;
                        foreach (Candidate candidate in structList)
                        {
                            candidate.resetVotes();
                        }
                    }

                    // Iterates through table, checking for firstchoices and tallying votes
                    foreach (DataRow myRow in myTable.Rows)
                    {
                        for (int i = 0; i < structList.Capacity - (1 + numEliminated + Math.Abs(round - 2)); i++)
                        {

                            if (myRow.Field<string>("firstChoice") == structList[i].getName() && !structList[i].eliminatedFromPlay)
                            {
                                structList[i].addVote();
                                myCalculator.totalVotes++;
                            }
                            // votes from rounds past 1 are only counted in their respective rounds
                            else if (myRow.Field<string>("secondChoice") == structList[i].getName() && !structList[i].eliminatedFromPlay && round >= 2)
                            {
                                structList[i].addVote();
                                myCalculator.totalVotes++;
                            }
                            else if (myRow.Field<string>("thirdChoice") == structList[i].getName() && !structList[i].eliminatedFromPlay && round == 3)
                            {
                                structList[i].addVote();
                                myCalculator.totalVotes++;
                            }
                        }
                    }
                    // lowestVotes is initialized with the totalVotes (will be overwritten shortly)
                    myCalculator.lowestVotes = myCalculator.totalVotes;
                    // Displays voting tallies after round is counted
                    foreach (Candidate candidate in structList)
                    {
                        Console.Write(candidate.getName());
                        Console.Write(" received ");
                        Console.Write(candidate.getVotes());
                        Console.WriteLine(" votes!");
                        myWriter.Write(candidate.getName());
                        myWriter.Write(" received ");
                        myWriter.Write(candidate.getVotes());
                        myWriter.WriteLine(" votes!");
                        myWriter.Flush();

                    }
                    // Displays voting percentages after round is counted
                    foreach (Candidate candidate in structList)
                    {
                        double myPercentage = Convert.ToSingle(candidate.getVotes()) / Convert.ToSingle(myCalculator.totalVotes) * 100.0;
                        Console.Write(candidate.getName());
                        Console.Write(" received ");
                        Console.Write(Convert.ToSingle(myPercentage));
                        Console.WriteLine("% of the votes!");
                        myWriter.Write(candidate.getName());
                        myWriter.Write(" received ");
                        myWriter.Write(Convert.ToSingle(myPercentage));
                        myWriter.WriteLine("% of the votes!");
                        myWriter.Flush();

                    }

                    // Checks for winners and eliminations
                    foreach (Candidate candidate in structList)
                    {
                        // winner calculation
                        if (candidate.getVotes() >= (0.5 * myCalculator.totalVotes))
                        {
                            Console.Write(candidate.getName());
                            Console.WriteLine(" Wins!");
                            myWriter.Write(candidate.getName());
                            myWriter.WriteLine(" Wins! ");
                            myWriter.Flush();

                            myCalculator.winnerDeclared = true;
                            fileLoaded = true;
                        }
                        //candidates that have zero votes
                        else if (candidate.getVotes() == 0)
                        {
                            candidate.eliminatedFromPlay = true;
                            Console.Write(candidate.getName());
                            Console.WriteLine(" has been eliminated.");
                            myWriter.Write(candidate.getName());
                            myWriter.WriteLine(" has been eliminated.");
                            myWriter.Flush();
                            numEliminated++;
                        }
                        //candidates that have the lowest number of votes
                        else if (candidate.getVotes() <= myCalculator.lowestVotes)
                        {
                            myCalculator.lowestVotes = candidate.getVotes();
                        }
                    }
                    // Those with no votes are stricken from the list
                    structList.RemoveAll(hasZeroVotes);
                    // Finds candidates who have or tie for the lowest amount of votes (non-zero) and marks them as such
                    foreach (Candidate candidate in structList)
                    {
                        if (candidate.getVotes() > 0 && candidate.getVotes() <= myCalculator.lowestVotes)
                        {
                            candidate.hasLowestVoteNum = true;
                        }
                    }
                    // Eliminates candidates who has the lowest number of votes (non-zero)
                    foreach (Candidate candidate in structList)
                    {
                        if (candidate.hasLowestVoteNum && (round != 2 && round != 3))
                        {
                            candidate.eliminatedFromPlay = true;
                            Console.Write(candidate.getName());
                            Console.WriteLine(" has been eliminated.");
                            myWriter.Write(candidate.getName());
                            myWriter.WriteLine(" has been eliminated.");
                            myWriter.Flush();
                            numEliminated++;
                        }
                    }
                    // excluding an ending tie, candidates with the lowest number of votes (non-zero) are eliminated
                    if (structList.Count > 2)
                        structList.RemoveAll(hasLowestVotes);
                }
                // user is prompted for another election calculation
                Console.WriteLine("Would you like to calculate another election? (yes/no): ");
                string tryAgain = Console.ReadLine();
                if (tryAgain != "yes")
                {
                    inUse = false;
                }
            }
        }        
    }
}
