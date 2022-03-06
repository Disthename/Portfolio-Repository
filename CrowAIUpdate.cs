using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CrowAIUpdate : MonoBehaviour
{
    public float speed = 10.0f;
    private Vector2 target;
    private Vector2 position;

    GameObject DriverTarget = null;
    public bool NextTo;
    private bool Touching;

    public Rigidbody2D ab;
    public Animator anim;

    // Start is called before the first frame update
    void Start()
    {
        DriverTarget = GameObject.Find("NPCDriver");

        target = new Vector2(8.35f, 2.65f);
        position = gameObject.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        // move away from driver
        if (NextTo)
        {
            GetComponent<BoxCollider2D>().isTrigger = true;
            float step = speed * Time.deltaTime;
            transform.position = Vector2.MoveTowards(transform.position, target, step);
        }
        // move towards driver until colliding with the driver
        else if (!Touching)
        {
            GetComponent<BoxCollider2D>().isTrigger = false;
            float step = speed * Time.deltaTime;
            transform.position = Vector2.MoveTowards(transform.position, DriverTarget.transform.position, step);
        }
    }

    void OnCollisionEnter2D(Collision2D other)
    {
        if (other.gameObject == DriverTarget)
        {
            Touching = true;
        }
    }

    void OnCollisionExit2D(Collision2D other)
    {
        if (other.gameObject == DriverTarget)
        {
            Touching = false;
        }
    }
}
