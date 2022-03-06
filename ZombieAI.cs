using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ZombieAI : BaseEnemyAI
{
    //Tracks current state
    public bool HostileState;
    protected SphereCollider DetectionSphere;
    private bool PlayerCheck;
    [SerializeField]
    protected AudioSource MySource;
    //Should be set within editor
    public float DetectionRadius;
    public float HitPoints;
    public float AttackRange;
    public float NextTimeToFire = 0.5f;
    public float AttackSpeed = 1.0f;
    public float Damage = 1.0f;
    public Transform AttackPoint;
    [SerializeField]
    protected AudioClip[] MyClips;
    public LayerMask EnemyLayer;
    Ray ray;

    protected override void Start()
    {
        base.Start();
        if (DetectionSphere == null)
        {
            DetectionSphere = GetComponent<SphereCollider>();
        }
        DetectionSphere.radius = DetectionRadius;
        if (MySource == null)
        {
            MySource = GetComponent<AudioSource>();
        }
    }
    private void Update()
    {
        //Sets AI in a new random direction if they are nearly at their end point (and aren't chasing the player)
        if (AIPathing.remainingDistance <= 0.25f && HostileState == false)
        {
            WanderLust();
        }
        //Chases and attacks player
        if(HostileState == true)
        {
            AIPathing.destination = Player.transform.position;
            //Checks if player is within attacking range
            PlayerCheck = Physics.CheckSphere(AttackPoint.position, AttackRange, PlayerLayer);
            if (PlayerCheck == true && Time.time >= NextTimeToFire)
            {
                Attack();
                NextTimeToFire = Time.time + 1f / AttackSpeed;
            }
            else if (PlayerCheck == false)
                EnemyAnim.SetBool("Attacking", false);
        }
    }


    //When player enters range
    private void OnTriggerEnter(Collider other)
    {
        //if Player is within radius and sight line is not blocked
        if (other.gameObject == Player && WithinSight(ray))
        {
            Hostile();
        }
    }

    //While player is in range
    private void OnTriggerStay(Collider other)
    {
        if(other.gameObject == Player && HostileState == false && WithinSight(ray))
        {
            Hostile();
        }
        if(other.gameObject == Player && HostileState == true && !WithinSight(ray))
        {
            Passive();
        }
    }
    private void FixedUpdate()
    {
        ray = new Ray(transform.position+Vector3.up, Player.transform.position - transform.position);
        Debug.DrawRay(transform.position+Vector3.up, Player.transform.position-transform.position);
    }
    public bool WithinSight(Ray ray)
    {
        RaycastHit hitData;

        if (Physics.Raycast(ray, out hitData, DetectionRadius) && hitData.collider == Player.GetComponent<Collider>())
        {
                //Debug.Log("Player in Sight!");
                //Debug.Log(hitData.collider);
                return true;
        }
        else 
        {
            //Debug.Log(hitData.collider);
            //Debug.Log("Out of sight");
            return false;
        }
    }

    //When player exits range
    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject == Player)
        {
            Passive();
        }
    }

    override public void Hostile()
    {
        HostileState = true;
        Debug.Log("Hostile Mode");
        AIPathing.destination = Player.transform.position;
        EnemyAnim.SetBool("Player Found", true);
        AIPathing.speed = 2.55f;
        MySource.clip = MyClips[1];
        MySource.Play();
    }

    override public void Passive()
    {
        HostileState = false;
        Debug.Log("Passive Mode");
        EnemyAnim.SetBool("Player Found", false);
        AIPathing.speed = 1.15f;
        MySource.clip = MyClips[0];
        MySource.Play();
    }

    override public void Attack()
    {
        EnemyAnim.SetBool("Attacking", true);
        PlayerHealth.reduceHealth(Damage);
        if (PlayerHealth.getHealth() <= 0.0f)
        {
            PlayerHealth.gameOver();
        }
        Debug.Log("Damage Taken");
        MySource.PlayOneShot(MyClips[2]);
    }



    public void WanderLust()
    {
        EnemyAnim.SetBool("On The Move", true);
        AIPathing.destination = new Vector3(Random.value * 8, AITrans.position.y, Random.value * 8);
        Debug.Log(AIPathing.destination);
        EnemyAnim.SetBool("Player Found", false);
    }

}
