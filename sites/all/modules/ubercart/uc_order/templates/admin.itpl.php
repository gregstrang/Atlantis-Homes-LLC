<?php
// $Id: admin.itpl.php,v 1.4.4.3 2009/04/18 07:29:18 islandusurper Exp $

/**
 * This file is the default admin notification template for Ubercart.
 */
?>

<p>
<?php echo t('Order number:'); ?> [order-admin-link]<br />
<?php echo t('Customer:'); ?> [order-first-name] [order-last-name] - [order-email]<br />
<?php echo t('Order total:'); ?> [order-total]<br />
<?php echo t('Shipping method:'); ?> [order-shipping-method]
</p>

<p>
<?php echo t('Products:'); ?><br />
<?php
$context = array(
  'location' => 'order-invoice-admin',
);
foreach ($products as $product) {
  $price_info = array(
    'price' => $product->price,
    'qty' => $product->qty,
  );
  $context['subject'] = array(
    'order_product' => $product,
  );
?>
- <?php echo $product->qty; ?> x <?php echo $product->title .' - '. uc_price($price_info, $context); ?><br />
&nbsp;&nbsp;<?php echo t('SKU: ') . $product->model; ?><br />
    <?php if (is_array($product->data['attributes']) && count($product->data['attributes']) > 0) {?>
    <?php foreach ($product->data['attributes'] as $key => $value) {
      echo '&nbsp;&nbsp;'. $key .': '. $value .'<br />';
    } ?>
    <?php } ?>
<br />
<?php } ?>
</p>

<p>
<?php echo t('Order comments:'); ?><br />
[order-comments]
</p>
