using System;
using System.Collections.Generic;
using System.Text;


namespace RankedBallotSystem
{
    struct Ballot
    {
        string voterID;
        string firstChoice;
        string secondChoice;
        string thirdChoice;
    }
    class BallotReader
    {
        Ballot myBallot;
    }
}
