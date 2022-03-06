using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPCDriverScript : MonoBehaviour
{
    public float health;
    public float damageRadius;
    public Rigidbody2D guyPhys;
    public Transform transform;
    public LayerMask enemyLayer;
    
    // Start is called before the first frame update
    void Start()
    {
        guyPhys = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        checkForDamage();
    }
    
    void checkForDamage()
    {
        Collider2D collider = Physics2D.OverlapCircle(transform.position, damageRadius, enemyLayer);
        if (collider != null)
        {
            health = health - 1;
            if (health <= 0)
            {
                Destroy(this.gameObject);
            }
        }
    }
}
