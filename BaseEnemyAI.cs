using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class BaseEnemyAI : MonoBehaviour
{
    //Variables that involve all possible enemy types
    public Transform AITrans;
    public GameObject Player;
    public NavMeshAgent AIPathing;
    public Animator EnemyAnim;
    public Rigidbody EnemyBody;
    public LayerMask PlayerLayer;
    public Health PlayerHealth;



    protected virtual void Start()
    {
        //Setting components if they are not set properly in editor
        if (AITrans == null)
        {
            AITrans = GetComponent<Transform>();
        }
        if (AIPathing == null)
        {
            AIPathing = GetComponent<NavMeshAgent>();
        }
        if (Player == null)
        {
            Player = GameObject.FindGameObjectWithTag("Player");
            PlayerHealth = Player.GetComponent<Health>();
        }
        if (EnemyBody == null)
        {
            EnemyBody = GetComponent<Rigidbody>();
        }
        if (EnemyAnim == null)
        {
            EnemyAnim = GetComponent<Animator>();
        }
    }
    
    // Update is called once per frame
    void Update()
    {

    }
    virtual public void Hostile() { }
    virtual public void Passive() { }
    virtual public void Attack() { }
    private void OnCollisionEnter(Collision collision)
    {
        
    }
    private void OnCollisionExit(Collision collision)
    {
        
    }

}
